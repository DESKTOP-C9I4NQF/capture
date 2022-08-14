#!/bin/bash
exec 3< /usr/share/dict/words

while read -r -u 3
do
	out=$(gpg -d --batch --passphrase "$REPLY" secret.gpg  2>/dev/null)
	echo $REPLY
	if [ "$out" ]; then
		printf "password is \"$out\" for text: \"$REPLY\"\n"
		exit
	fi
	exit
done
