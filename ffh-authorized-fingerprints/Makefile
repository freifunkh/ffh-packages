include $(TOPDIR)/rules.mk

PKG_NAME:=ffh-authorized-fingerprints
PKG_VERSION:=1

PKG_LICENSE:=BSD-2-Clause

PKG_BUILD_DEPENDS := respondd

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/ffh-authorized-fingerprints
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Add abbreviated authorized ssh key fingerprints to respondd
  DEPENDS:=+gluon-core +libecdsautil +libgluonutil +respondd
endef

define Package/ffh-authorized-fingerprints/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib/respondd/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ffh-authorized-fingerprints $(1)/usr/sbin/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/respondd/librespondd.so $(1)/usr/lib/respondd/authorized_fp.so
endef

$(eval $(call BuildPackage,ffh-authorized-fingerprints))
