# On Android device:
# 1. open Termux
# 2. Install: pkg install openssh
# 3. Authorize host in ~/.ssh/authorized_keys
# 4. Run: sshd
# On Host:
# Run: make rsync-dry
# Run: make rsync
# On Android:
# 5. Close sshd: pkill sshd
rsync:
	rsync -avh --delete --size-only --info=progress2 /mnt/data/Igrek/mp3/ termux:/data/data/com.termux/files/home/sd/mp3

rsync-dry:
	rsync -rvn --size-only --delete /mnt/data/Igrek/mp3/ termux:/data/data/com.termux/files/home/sd/mp3

