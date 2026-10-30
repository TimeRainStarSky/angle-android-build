MODPATH="${0%/*}"
ns_mount() {
  if nsenter -t 1 -m -- mount -o bind "$MODPATH/$1" "/$1" 2>/dev/null; then
    return 0
  fi
  mount -o bind "$MODPATH/$1" "/$1"
}
if [ -f "$MODPATH/skip_mount" ]; then
  ns_mount system/lib64/libEGL_angle.so
  ns_mount system/lib64/libGLESv1_CM_angle.so
  ns_mount system/lib64/libGLESv2_angle.so
fi
