include $(TOPDIR)/rules.mk

PKG_NAME:=ffh-watchdog
PKG_VERSION:=1

PKG_LICENSE:=BSD-2-Clause

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/ffh-watchdog
  SECTION:=admin
  CATEGORY:=Administration
  TITLE:=Watchdog trigger based on gluon-state-check
  DEPENDS:=+gluon-core +libubus +ffh-ntpd-gluon-state-check
endef

define Package/ffh-watchdog/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/autoupdater/abort.d/
	$(INSTALL_DIR) $(1)/usr/lib/autoupdater/upgrade.d/
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ffh-watchdog $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libwatchdog.so $(1)/usr/lib/
	$(INSTALL_BIN) files/etc/init.d/ffh-watchdog $(1)/etc/init.d/ffh-watchdog
	$(INSTALL_BIN) files/usr/lib/autoupdater/abort.d/25watchdog $(1)/usr/lib/autoupdater/abort.d/25watchdog
	$(INSTALL_BIN) files/usr/lib/autoupdater/upgrade.d/25watchdog $(1)/usr/lib/autoupdater/upgrade.d/25watchdog
endef

$(eval $(call BuildPackage,ffh-watchdog))
