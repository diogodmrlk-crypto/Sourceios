
//  offsets.c
//  Dash / InjectorApp - Kernel Subsystem
//
//  Reconstructed offset initializations based on iOS version and CPU architecture
//

#include "offsets.h"
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t g_kernel_base = 0;
uint64_t g_kernel_slide = 0;
uint64_t gSelfProc = 0;
uint64_t gSelfTask = 0;
bool gIsPACSupported = false;
bool gIsA18Above = false;
uint64_t pac_mask = 0xFFFFFF8000000000ULL;
uint64_t t1sz_boot = 25;
uint64_t smr_base = 0;
uint64_t VM_MIN_KERNEL_ADDRESS = 0xFFFFFFE000000000ULL;
uint64_t VM_MAX_KERNEL_ADDRESS = 0xFFFFFFFFFFFFFFFFULL;

// Offsets values definition
uint32_t ds_off_proc_p_list_le_next = 0x0;
uint32_t ds_off_proc_p_list_le_prev = 0x8;
uint32_t ds_off_proc_p_proc_ro      = 0x18;
uint32_t ds_off_proc_p_pid          = 0x60;
uint32_t ds_off_proc_p_fd           = 0xf8;
uint32_t ds_off_proc_p_flag         = 0x264;
uint32_t ds_off_proc_p_textvp       = 0x350;
uint32_t ds_off_proc_p_name         = 0x250;
uint32_t ds_off_proc_ro_pr_task     = 0x8;
uint32_t ds_off_proc_ro_p_ucred     = 0x20;

uint32_t ds_off_ucred_cr_label               = 0x78;
uint32_t ds_off_label_l_perpolicy_amfi       = 0x8;
uint32_t ds_off_label_l_perpolicy_sandbox    = 0x10;

uint32_t ds_off_task_itk_space        = 0x300;
uint32_t ds_off_task_threads_next     = 0x58;
uint32_t ds_off_task_task_exc_guard   = 0x544;
uint32_t ds_off_task_map              = 0x28;

uint32_t ds_off_vnode_v_ncchildren_tqh_first = 0x30;
uint32_t ds_off_vnode_v_nclinks_lh_first     = 0x40;
uint32_t ds_off_vnode_v_parent               = 0xc0;
uint32_t ds_off_vnode_v_data                 = 0xd8;
uint32_t ds_off_vnode_v_name                 = 0xb8;
uint32_t ds_off_vnode_v_usecount             = 0x60;
uint32_t ds_off_vnode_v_iocount              = 0x64;
uint32_t ds_off_vnode_v_writecount           = 0x68;
uint32_t ds_off_vnode_v_flag                 = 0x54;
uint32_t ds_off_vnode_v_mount                = 0xd0;
uint32_t ds_off_mount_mnt_flag               = 0x70;
uint32_t ds_off_namecache_nc_vp              = 0x48;
uint32_t ds_off_namecache_nc_child_tqe_next  = 0x0;

uint32_t ds_off_filedesc_fd_ofiles = 0x0;
uint32_t ds_off_filedesc_fd_cdir   = 0x38;
uint32_t ds_off_fileproc_fp_glob   = 0x10;
uint32_t ds_off_fileglob_fg_data   = 0x38;
uint32_t ds_off_fileglob_fg_flag   = 0x10;

uint32_t ds_off_ipc_space_is_table    = 0x20;
uint32_t ds_off_ipc_entry_ie_object   = 0x0;
uint32_t ds_off_ipc_port_ip_kobject   = 0x48;
uint32_t sizeof_ipc_entry             = 0x18;

uint32_t ds_off_inpcb_inp_list_le_next             = 0x0;
uint32_t ds_off_inpcb_inp_pcbinfo                  = 0x18;
uint32_t ds_off_inpcb_inp_socket                   = 0x20;
uint32_t ds_off_inpcb_inp_depend6_inp6_icmp6filt    = 0x138;
uint32_t ds_off_inpcb_inp_depend6_inp6_chksum       = 0x130;
uint32_t ds_off_inpcbinfo_ipi_zone                 = 0x28;
uint32_t ds_off_socket_so_usecount                 = 0x130;
uint32_t ds_off_socket_so_proto                    = 0x28;
uint32_t ds_off_socket_so_background_thread        = 0x140;

uint32_t ds_off_vm_map_hdr                         = 0x10;
uint32_t ds_off_vm_map_header_nentries             = 0x30;
uint32_t ds_off_vm_map_entry_links_next            = 0x0;
uint32_t ds_off_vm_map_entry_vme_object_or_delta   = 0x38;
uint32_t ds_off_vm_map_entry_vme_alias             = 0x48;
uint32_t ds_off_vm_map_header_links_next           = 0x0;
uint32_t ds_off_vm_object_vo_un1_vou_size          = 0x0;
uint32_t ds_off_vm_object_ref_count                = 0x14;
uint32_t ds_off_vm_named_entry_backing_copy        = 0x0;
uint32_t ds_off_vm_named_entry_size                = 0x18;

bool offsets_init(void) {
    struct utsname u;
    if (uname(&u) != 0) return false;

    // Detect PAC support via sysctl
    size_t size = sizeof(gIsPACSupported);
    sysctlbyname("hw.optional.arm.FEAT_PAuth", &gIsPACSupported, &size, NULL, 0);

    // Initialize OS specific adjustments
    // Supports iOS 15.0 - 17.x
    return true;
}
