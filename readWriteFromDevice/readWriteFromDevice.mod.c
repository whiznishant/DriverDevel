#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x5f0e7c44, "module_layout" },
	{ 0xda8ade4a, "cdev_del" },
	{ 0x6049d54d, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe575deb3, "class_destroy" },
	{ 0xbd01872c, "device_create" },
	{ 0xdf28d2a3, "__class_create" },
	{ 0x8ff5ba3c, "cdev_add" },
	{ 0x513fa136, "cdev_init" },
	{ 0xa08bea2e, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7B9E2A09B0BF928C258ED9C");
