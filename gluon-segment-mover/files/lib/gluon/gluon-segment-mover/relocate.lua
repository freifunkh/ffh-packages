#!/usr/bin/lua
local public = "52ea839a5d105f1e4e4558f02a934bb04eb45c117c761a6166b5f02a0f98332e"

local devurandom = io.open("/dev/urandom","rb")
local b1,b2 = devurandom:read(2):byte(1,2)
local autoupdaterlockfile='/var/lock/autoupdater.lock'
local directorurl='http://hannover.freifunk.net/api/move/'
seed = b1 + (256 * b2)
math.randomseed(seed)
local delay = math.random(60)
function file_exists(name)
  local f=io.open(name,"r")
  if f~=nil then io.close(f) return true else return false end
end

local uci=require('simple-uci').cursor()
local o=uci:get('gluon','core','ignorerelocate')
if (o=='1') then
  io.write("Ignoring domain change request. Exiting.\n")
  do return end
end
io.write('Sleeping for ' .. delay .. ' seconds.\n')
os.execute('sleep ' .. delay)

local currentdomain=uci:get("gluon","core","domain")
local nodeid = require('gluon.util').node_id()

local url=directorurl .. nodeid
local response = io.popen("wget -q -O - '" .. url .. "'"):read('*a')
local manseg
local signature
manseg, signature = response:match("(.-)\n.+\n(.-)\n")
if (manseg == nil or signature == nil) then
	io.write("Servers answer is weird. Aborting.\n")
	do return end
end

local tempdomainfile = io.popen("mktemp"):read('*a'):match("(.-)\n.*")
os.execute("echo -n " .. manseg .. " > " .. tempdomainfile)
local returncode = io.popen("ecdsaverify -s " .. signature .. " -p " .. public .. " " .. tempdomainfile .. " && echo -n true || echo -n false"):read('*a')
os.execute("rm " .. tempdomainfile)

if (returncode ~= "true") then
	io.write("Signature mismatch. Exiting.\n")
	do return end
end

io.write('Current domain: ' .. currentdomain .. '\nNodeID: ' .. nodeid .. '\nrequested domain: ' .. manseg .. '\n')
if (manseg ~= "" or currentdomain=="ref" ) then
  newseg = manseg
end

if (currentdomain==newseg or newseg == "" or newseg == nil or newseg == "noop") then
  io.write("Do nothing..\n")
else
  if (file_exists('/lib/gluon/domains/' .. newseg .. '.json') == true) then
    -- This is not nice, but it works. nixio lacks flock...
    local alock = io.popen('lock -n ' .. autoupdaterlockfile .. ' 2>/dev/null && echo unlocked || echo locked'):read('*l')
    if (alock ~= 'unlocked') then
      io.write('Detected lock on ' .. autoupdaterlockfile .. '. Exiting.\n')
      do return end
    end
    os.execute('logger -s -t "gluon-segment-mover" -p 5 "Domain change requested. Moving to "' .. newseg)
    uci:set('gluon','core','domain',newseg)
    uci:save('gluon')
    uci:commit('gluon')
    os.execute('/usr/bin/gluon-reconfigure')
    io.popen('lock -u ' .. autoupdaterlockfile)
    io.popen('reboot')
  else
    io.write('Invalid Domain requested. I don\'t have ' .. newseg .. '.json')
    do return end
  end
end
