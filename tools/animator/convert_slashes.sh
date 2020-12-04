FILES=`find -name "*.ase"`

for f in $FILES
do
        sed -i 's/D:\\projects\\blunted2\\tools\\animator\\//g' $f
        sed -i 's/\\/\//g' $f
done
