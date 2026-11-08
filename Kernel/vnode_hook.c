//
//  vnode_hook.c
//  Dash / InjectorApp - Kernel Subsystem
//
//  Swaps v_data pointers between vnodes directly in kernel memory
//

#include "vnode_hook.h"
#include "offsets.h"
#include "kexploit.h"
#include <unistd.h>
#include <fcntl.h>

uint64_t get_vnode_for_path_by_chdir(const char *path) {
    int saved_fd = open(".", O_RDONLY);
    if (chdir(path) != 0) {
        if (saved_fd >= 0) close(saved_fd);
        return 0;
    }
    
    // Read current working directory vnode from proc->p_fd->fd_cdir
    uint64_t proc = proc_self();
    uint64_t p_fd = kread64(proc + ds_off_proc_p_fd);
    uint64_t cdir_vnode = kread64(p_fd + ds_off_filedesc_fd_cdir);
    
    if (saved_fd >= 0) {
        fchdir(saved_fd);
        close(saved_fd);
    }
    return cdir_vnode;
}

uint64_t vnode_redirect_folder(const char *target_path, const char *replacement_path) {
    uint64_t vnode_target = get_vnode_for_path_by_chdir(target_path);
    if (!vnode_target) return 0;
    
    uint64_t orig_v_data = kread64(vnode_target + ds_off_vnode_v_data);
    
    uint64_t vnode_replacement = get_vnode_for_path_by_chdir(replacement_path);
    if (!vnode_replacement) return 0;
    
    uint64_t new_v_data = kread64(vnode_replacement + ds_off_vnode_v_data);
    
    // Swap v_data in target vnode to point to replacement directory!
    kwrite64(vnode_target + ds_off_vnode_v_data, new_v_data);
    return orig_v_data;
}

void vnode_unredirect_folder(const char *target_path, uint64_t orig_v_data) {
    uint64_t vnode_target = get_vnode_for_path_by_chdir(target_path);
    if (vnode_target && orig_v_data) {
        kwrite64(vnode_target + ds_off_vnode_v_data, orig_v_data);
    }
}
