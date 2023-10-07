/**
 * pntr_app_libretro_vfs: pntr_app file system for libretro.
 *
 * Define a \c PNTR_APP_LIBRETRO_VFS method to retrieve libretro's virtual file system interface:
 *
 * @code
 *      struct retro_vfs_interface* myapp_get_vfs() {
 *          struct retro_vfs_interface_info vfs_interface_info;
 *          vfs_interface_info.required_interface_version = 1;
 *          vfs_interface_info.iface = NULL;
 *          if (environ_cb(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &vfs_interface_info)) {
 *              vfs = vfs_interface_info.iface;
 *          }
 *          else {
 *              vfs = NULL;
 *          }
 *      }
 *      #define PNTR_APP_LIBRETRO_VFS myapp_get_vfs()
 *      #include "extensions/pntr_app_libretro_vfs.h"
 * @endcode
 */

#ifndef PNTR_APP_LIBRETRO_VFS
#error "pntr_app_libretro_vfs: Need to define PNTR_APP_LIBRETRO_VFS as a method to grab the active VFS"
#endif

#ifndef PNTR_LOAD_FILE
    /**
     * Load a file using libretro's virtual file system.
     */
    unsigned char* pntr_app_libretro_load_file(const char* fileName, unsigned int* bytesRead) {
        struct retro_vfs_interface* vfs = PNTR_APP_LIBRETRO_VFS;
        if (vfs == NULL) {
            return NULL;
        }

        // Open the file
        struct retro_vfs_file_handle* file = vfs->open(fileName, RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);
        if (file == NULL) {
            if (bytesRead != NULL) {
                *bytesRead = 0;
            }
            return NULL;
        }

        // Get the size
        int64_t size = vfs->size(file);
        if (size <= 0) {
            vfs->close(file);
            if (bytesRead != NULL) {
                *bytesRead = 0;
            }
            return NULL;
        }

        // Prepare the data
        unsigned char* data = (unsigned char*)pntr_load_memory(size * sizeof(unsigned char) + 1);
        if (data == NULL) {
            vfs->close(file);
            if (bytesRead != NULL) {
                *bytesRead = 0;
            }
            return NULL;
        }

        // Read the file
        unsigned int bytes = (unsigned int)vfs->read(file, data, size);
        vfs->close(file);
        if (bytesRead != NULL) {
            *bytesRead = bytes;
        }

        return data;
    }
    #define PNTR_LOAD_FILE pntr_app_libretro_load_file
#endif

#ifndef PNTR_SAVE_FILE
    /**
     * Save a file using libretro's file system.
     */
    bool pntr_app_libretro_save_file(const char *fileName, const void *data, unsigned int bytesToWrite) {
        struct retro_vfs_interface* vfs = PNTR_APP_LIBRETRO_VFS;
        if (vfs == NULL) {
            return false;
        }

        // Open the file
        struct retro_vfs_file_handle* file = vfs->open(fileName, RETRO_VFS_FILE_ACCESS_WRITE, RETRO_VFS_FILE_ACCESS_HINT_NONE);
        if (file == NULL) {
            return false;
        }

        // Write the file
        int64_t bytesWritten = vfs->write(file, data, (uint64_t)bytesToWrite);
        if (bytesWritten <= 0) {
            vfs->close(file);
            return false;
        }

        // Flush the buffer and close the file.
        vfs->flush(file);
        vfs->close(file);

        return true;
    }
    #define PNTR_SAVE_FILE(fileName, data, bytesToWrite) pntr_app_libretro_save_file(fileName, data, bytesToWrite)
#endif
