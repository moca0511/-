# install
git clone -b rtklib_2.4.3 https://github.com/tomojitakasu/RTKLIB.git
cd ./RTKLIB/app/consapp
make
sudo cp str2str /usr/local/bin


sudo vi  /etc/udev/rules.d/99-com.rules
