# echo "#ifndef _APPLIB_"
# echo "#define _APPLIB_"
files=$(find * -type f -name *.h | sort)
for i in $files; do
	echo "#include <$i>"
done
# echo "#endif"