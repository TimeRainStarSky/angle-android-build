set_perm_recursive "$MODPATH/system/lib64" 0 0 0755 0644 u:object_r:system_lib_file:s0

chmod 0755 "$MODPATH/glinfo"
ui_print "- $("$MODPATH/glinfo")"

if [ -n "$KSU" ]; then
  touch "$MODPATH/skip_mount"
fi

if [ ! -d "$MODPATH/../../modules/$MODID" ]; then
  setprop persist.graphics.egl angle
  for i in angle_debug_package angle_gl_driver_all_angle; do
    settings delete global "$i" >&2
  done
fi
