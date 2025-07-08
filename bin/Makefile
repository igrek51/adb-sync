# On Android device:
# 1. open Termux
# 2. Install: pkg install openssh
# 3. Authorize host in ~/.ssh/authorized_keys.
# Copy key with `adb connect`,
# `adb push ~/.ssh/id_rsa.pub /sdcard/`
# Copy to ~/.ssh/authorized_keys
# 4. Run: sshd -D -d -p 8023
# On Host:
# Add to ~/.ssh/config
#Host termux
#  Hostname 192.168.0.37
#  User (CHECK WITH id)
#  Port 8023
#
# Run: make rsync-dry
# Run: make rsync
# On Android:
# 5. Close sshd: pkill sshd
rsync:
	rsync -rvh --delete --size-only --info=progress2 /opt/dump/mp3/ termux:/data/data/com.termux/files/home/shared/mp3

rsync-dry:
	rsync -rvhn --delete --size-only /opt/dump/mp3/ termux:/data/data/com.termux/files/home/shared/mp3

