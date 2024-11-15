sudo useradd garfield
# add garfield and rowan to the same group
sudo groupadd users
sudo adduser rowan users
sudo adduser garfield users

echo "Computer Science & Math | Username: rowan, Clone: garfield | 11/14/2024 | CPSC340, Fall 2024" > file1_11-14-24.txt
ls -l
sudo chmod g+r-wx file1_11-14-24.txt
ls -l
sudo chmod g+rw-x file1_11-14-24.txt

# make a link to the garfield user
ln -s ~/Documents/Code/OpSys/file1_11-14-24.txt /home/garfield/file1.txt

su --login garfield
# try to modify the file1_11-14-24.txt file

sudo chmod g-rwx file1_11-14-24.txt

umask 077
touch file2_11-14-2024.txt

su --login rowan
chown rowan file1_11-14-24.txt

