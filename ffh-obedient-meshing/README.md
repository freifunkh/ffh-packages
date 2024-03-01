# ffh-obedient-meshing

This package allows the router to scan for available mesh-networks in order to join the most prominent one in radio distance.

## Dependencies

This package relies on `gluon-state-online` to tell whether it has mesh partners.
It relies on `micron.d` to do this every minute.
In case it doesn't, it relies on `iw` and `iwinfo` to scan for wireless networks nearby and join one.
Lastly this only makes sense to be deployed in a multi-domain-setup.

##what it does

### when there are mesh neighbors

The device stays in that domain, where it and its neighbors currently are.

### when there aren't

The file `/var/guon/state/has_neighbours` is missing.
It scans for mesh networks, and checks for a network with a `mesh_id` matching one of the known domains meshes.
It does this on all available radios hence all available bands.

In case it finds none, the device will try again a minute later, until it eventually does.

In case it finds more than one, it determines the locally most popular (whichever it encounters most often) and changes its domain in order to match it.

It reconfigures without reboot using `gluon-switch-domain` and should have mesh neighbors a minute later.

### on boot

Domains are configured and stored across (unrelated) reboots.
As the device comes up it is part of the last domain configured, like a regular device.
And only if it does not have mesh neighbors, it does its job.

## drawbacks

Mesh networks do only provide a `mesh_id` which is shared across multiple domains related to one primary.
This package does not know about which pretty domains are near.
It therefore joins the primary one; which leads to devices in e.g. `dom14` instead of `Nordstadt` in Hanover.

Devices do not mark that they're meshing obediently; this could lead to unexpected behavior for mesh-clouds without Internet-access. As they technically have neighbors and therefore do not need to switch domains. This is for now intended behavior and matter to discuss,  but not part of this first implementation.

