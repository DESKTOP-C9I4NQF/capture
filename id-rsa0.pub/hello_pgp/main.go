package main

import (
	"fmt"
	"os"
	"syscall"
	"bufio"
	"io"
)

// check for errors
func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	argv := make([]string, 6)
	argv[0] = "/usr/bin/gpg"
	argv[1] = "--batch"
	argv[2] = "--passphrase"
	argv[4] = "-d"
	argv[5] = "secret.gpg"

	fild, err := os.Open("/usr/share/dict/words")
	check(err)

	reader := bufio.NewReader(fild)
	for {
		line,_, err := reader.ReadLine()
		if err == io.EOF {
			break
		}

		argv[3] = string(line)
		fmt.Printf("%s\n", line)

		devnull, err := os.Open("/dev/null")
		check(err)

		r_end, w_end,_ := os.Pipe()

		// execute target binary as child process and set stdout to pipe and stderr to
		// `/dev/null'
		childPid,_ := syscall.ForkExec(argv[0], argv, &syscall.ProcAttr{
			Env: nil,
			Files: []uintptr{0, w_end.Fd(), devnull.Fd()},
		})

		w_end.Close()
		devnull.Close()

		file_buf := bufio.NewReader(r_end)
		data , _, err := file_buf.ReadLine()

		// close unused file descriptor
		r_end.Close()
		syscall.Wait4(childPid, nil, 0, nil)

		// found the flag
		if data != nil {
			fmt.Printf("flag is : `%s'\n", data)
			break
		}
	}
	fild.Close()
}
