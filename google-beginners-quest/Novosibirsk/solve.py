#!/usr/bin/env  python3

def func(p):
    targetstr = ""
    for reg in p:
        targetstr += chr(reg - 0xCafe)

    return targetstr


def main():
    p = [0] * 12
    p[0] = 52037
    p[6] = 52081 
    p[5] = 52063 
    p[1] = 52077 
    p[9] = 52077 
    p[10] = 52080 
    p[4] = 52046 
    p[3] = 52066 
    p[8] = 52085 
    p[7] = 52081 
    p[2] = 52077 
    p[11] = 52066

    print(func(p))

if __name__ == "__main__":
    main()
