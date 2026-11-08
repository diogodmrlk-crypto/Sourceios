//
//  vnode_hook.h
//  Dash / InjectorApp - Kernel Subsystem
//
//  In-kernel vnode redirection interface
//

#ifndef vnode_hook_h
#define vnode_hook_h

#include <stdint.h>
#include <stdbool.h>

uint64_t get_vnode_for_path_by_chdir(const char *path);
uint64_t get_vnode_for_path_by_open(const char *path);
uint64_t get_vnode_by_fd(int fd);
uint64_t get_rootvnode(void);

uint64_t vnode_redirect_folder(const char *target_path, const char *replacement_path);
void vnode_unredirect_folder(const char *target_path, uint64_t orig_v_data);

uint64_t vnode_redirect_file(const char *target_file, const char *replacement_file);
void vnode_unredirect_file(const char *target_file, uint64_t orig_v_data);

#endif /* vnode_hook_h */
