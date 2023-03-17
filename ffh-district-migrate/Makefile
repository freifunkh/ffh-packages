include $(TOPDIR)/rules.mk

PKG_NAME:=ffh-district-migrate
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ffh-district-migrate
  SECTION:=gluon
  CATEGORY:=Gluon
  DEPENDS:=+ffh-district-core
  TITLE:=Migrate gluon nodes to ffh-district districts by asking a http server for the right district
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(call GluonSrcDiet,./luasrc,$(PKG_BUILD_DIR)/luadest/)
endef

define Package/ffh-district-migrate/install
	$(CP) ./files/* $(1)/
	$(CP) $(PKG_BUILD_DIR)/luadest/* $(1)/
endef

$(eval $(call BuildPackage,ffh-district-migrate))
