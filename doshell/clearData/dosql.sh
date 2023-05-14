# /bin/sh

sqlfile=$1
dbfile=$2
appname=$3

if [ x"" = x"$1" ];then
echo "need input sqlfile, ./dosql.sh clearFaceAndAuth.sql"
return
fi

if [ x"" = x"$2" ];then
echo "need input dbfile ./dosql.sh clearFaceAndAuth.sql /data/zyep_dctr/data/auth.db"
return 
fi

if [ x"" = x"$3" ];then
echo "need input dbfile ./dosql.sh clearFaceAndAuth.sql /data/zyep_dctr/data/auth.db zyep_dctr"
return 
fi

echo "delall face and auth info, del versions"
sqlite3 -init ${sqlfile} ${dbfile} << CMD
.exit
CMD

killall ${appname}

echo "done"
