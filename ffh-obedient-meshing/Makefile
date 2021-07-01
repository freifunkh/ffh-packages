include $(TOPDIR)/rules.mk

PKG_NAME:=ffh-obedient-meshing
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(TOPDIR)/../package/gluon.mk

define Package/ffh-obedient-meshing
  SECTION:=ffh
  CATEGORY:=FFH
  TITLE:=Automatically switch to the most popular domain in wifi reach, as soon as no mesh neighbours are available.
  DEPENDS:=+gluon-core +gluon-state-check +iw +libiwinfo-lua +micrond @GLUON_MULTIDOMAIN
  MAINTAINER:=Freifunk Hannover <dev@hannover.freifunk.net>
endef

define Package/ffh-obedient-meshing/description
  This package checks periodically, whether the router has mesh-neighbours.
  If it doesn't,
  it scans for mesh-networks,
  filters out the ones that are not part of its site and domainconf,
  joins the most popular across all radios, regardless of their band.
  It does so by switching its own domain.
endef

$(eval $(call BuildPackageGluon,$(PKG_NAME)))
