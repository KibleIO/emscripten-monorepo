#/bin/bash

mv /root/working/rana/output/* ./kib
sed -i 's@src=@src=/kib/@g' /root/working/kib/index.html
tail -f /dev/null