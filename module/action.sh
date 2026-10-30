MODPATH="${0%/*}"
EGL=$(getprop persist.graphics.egl)
if [ "$EGL" = angle ]; then
  EGL=
else
  EGL=angle
fi
setprop persist.graphics.egl "$EGL"
sh "$MODPATH/service.sh"
grep -oE '\[.*\]' "$MODPATH/module.prop"
