include $(TOPDIR)/rules.mk

PKG_NAME:=ffh-ntpd-gluon-state-check
PKG_VERSION:=2

include $(TOPDIR)/../package/gluon.mk

define Package/ffh-ntpd-gluon-state-check
  TITLE:=Hotplug for gluon-state-check into ntpd
  DEPENDS:=+gluon-core +gluon-state-check
endef

$(eval $(call BuildPackageGluon,ffh-ntpd-gluon-state-check))
