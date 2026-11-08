//
//  offsets.h
//  Dash / InjectorApp - Kernel Subsystem
//
//  Reconstructed kernel struct offsets for iOS 15 - 17+
//

#ifndef offsets_h
#define offsets_h

#include <stdint.h>
#include <stdbool.h>

// Global kernel structures and flags
extern uint64_t g_kernel_base;
extern uint64_t g_kernel_slide;
extern uint64_t gSelfProc;
extern uint64_t gSelfTask;
extern bool gIsPACSupported;
extern bool gIsA18Above;
extern uint64_t pac_mask;
extern uint64_t t1sz_boot;
extern uint64_t smr_base;
extern uint64_t VM_MIN_KERNEL_ADDRESS;
extern uint64_t VM_MAX_KERNEL_ADDRESS;

// Process (proc) offsets
extern uint32_t ds_off_proc_p_list_le_next;
extern uint32_t ds_off_proc_p_list_le_prev;
extern uint32_t ds_off_proc_p_proc_ro;
extern uint32_t ds_off_proc_p_pid;
extern uint32_t ds_off_proc_p_fd;
extern uint32_t ds_off_proc_p_flag;
extern uint32_t ds_off_proc_p_textvp;
extern uint32_t ds_off_proc_p_name;
extern uint32_t ds_off_proc_ro_pr_task;
extern uint32_t ds_off_proc_ro_p_ucred;

// Credential and Sandbox offsets
extern uint32_t ds_off_ucred_cr_label;
extern uint32_t ds_off_label_l_perpolicy_amfi;
extern uint32_t ds_off_label_l_perpolicy_sandbox;

// Task and Thread offsets
extern uint32_t ds_off_task_itk_space;
extern uint32_t ds_off_task_threads_next;
extern uint32_t ds_off_task_task_exc_guard;
extern uint32_t ds_off_task_map;

extern uint32_t ds_off_thread_t_tro;
extern uint32_t ds_off_thread_ro_tro_proc;
extern uint32_t ds_off_thread_ro_tro_task;
extern uint32_t ds_off_thread_machine_upcb;
extern uint32_t ds_off_thread_machine_contextdata;
extern uint32_t ds_off_thread_ctid;
extern uint32_t ds_off_thread_options;
extern uint32_t ds_off_thread_mutex_lck_mtx_data;
extern uint32_t ds_off_thread_machine_kstackptr;
extern uint32_t ds_off_thread_machine_jop_pid;
extern uint32_t ds_off_thread_machine_rop_pid;
extern uint32_t ds_off_thread_guard_exc_info_code;
extern uint32_t ds_off_thread_mach_exc_info_code;
extern uint32_t ds_off_thread_mach_exc_info_os_reason;
extern uint32_t ds_off_thread_mach_exc_info_exception_type;
extern uint32_t ds_off_thread_ast;
extern uint32_t ds_off_thread_task_threads_next;

// File descriptor & vnode offsets
extern uint32_t ds_off_filedesc_fd_ofiles;
extern uint32_t ds_off_filedesc_fd_cdir;
extern uint32_t ds_off_fileproc_fp_glob;
extern uint32_t ds_off_fileglob_fg_data;
extern uint32_t ds_off_fileglob_fg_flag;

extern uint32_t ds_off_vnode_v_ncchildren_tqh_first;
extern uint32_t ds_off_vnode_v_nclinks_lh_first;
extern uint32_t ds_off_vnode_v_parent;
extern uint32_t ds_off_vnode_v_data;
extern uint32_t ds_off_vnode_v_name;
extern uint32_t ds_off_vnode_v_usecount;
extern uint32_t ds_off_vnode_v_iocount;
extern uint32_t ds_off_vnode_v_writecount;
extern uint32_t ds_off_vnode_v_flag;
extern uint32_t ds_off_vnode_v_mount;
extern uint32_t ds_off_mount_mnt_flag;
extern uint32_t ds_off_namecache_nc_vp;
extern uint32_t ds_off_namecache_nc_child_tqe_next;

// IPC offsets
extern uint32_t ds_off_ipc_space_is_table;
extern uint32_t ds_off_ipc_entry_ie_object;
extern uint32_t ds_off_ipc_port_ip_kobject;
extern uint32_t sizeof_ipc_entry;

// Socket & inpcb offsets (kfd exploit primitives)
extern uint32_t ds_off_inpcb_inp_list_le_next;
extern uint32_t ds_off_inpcb_inp_pcbinfo;
extern uint32_t ds_off_inpcb_inp_socket;
extern uint32_t ds_off_inpcb_inp_depend6_inp6_icmp6filt;
extern uint32_t ds_off_inpcb_inp_depend6_inp6_chksum;
extern uint32_t ds_off_inpcbinfo_ipi_zone;
extern uint32_t ds_off_socket_so_usecount;
extern uint32_t ds_off_socket_so_proto;
extern uint32_t ds_off_socket_so_background_thread;

// VM map offsets
extern uint32_t ds_off_vm_map_hdr;
extern uint32_t ds_off_vm_map_header_nentries;
extern uint32_t ds_off_vm_map_entry_links_next;
extern uint32_t ds_off_vm_map_entry_vme_object_or_delta;
extern uint32_t ds_off_vm_map_entry_vme_alias;
extern uint32_t ds_off_vm_map_header_links_next;
extern uint32_t ds_off_vm_object_vo_un1_vou_size;
extern uint32_t ds_off_vm_object_ref_count;
extern uint32_t ds_off_vm_named_entry_backing_copy;
extern uint32_t ds_off_vm_named_entry_size;

bool offsets_init(void);

#endif /* offsets_h */
