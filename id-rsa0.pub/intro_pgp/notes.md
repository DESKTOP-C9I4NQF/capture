# PGP Pretty Good Privacy

> Installation of PGP on ubuntu
```
	sudo apt-get install gnupg
``
> Generate gpg key
```
	gpg --gen-key
```

> encrypt a text
```
echo "hello id0!" | gpg -e --armor
```

> Decrypt a text
```
cat signed\_message.asc | gpg -d
```

> CTF flag
```
Thank you Phil Zimmermann!
```
