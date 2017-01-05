#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9fdb097c, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x27e1a049, "printk" },
	{ 0x16653c87, "__register_chrdev" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xa9e215f4, "module_put" },
	{ 0x37a0cba, "kfree" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x30caf905, "kmem_cache_alloc" },
	{ 0x2580243a, "kmalloc_caches" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

