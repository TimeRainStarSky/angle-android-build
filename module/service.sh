MODPATH="${0%/*}"
X01="$(echo -e "\x01")"
EGL=$(getprop persist.graphics.egl)
GLINFO="$("$MODPATH/glinfo")"
if [ "$EGL" = angle ]; then
  sed -i "s$X01\[.*\]$X01[✅ ANGLE 已全局启用 $GLINFO]$X01" "$MODPATH/module.prop"
else
  sed -i "s$X01\[.*\]$X01[❎ ANGLE 已全局禁用 $GLINFO]$X01" "$MODPATH/module.prop"
fi
