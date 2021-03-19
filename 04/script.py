#!/usr/bin/python3


def apply(operation, first, second):
    if operation == '-':
        return first - second
    elif operation == '+':
        return first + second
    elif operation == '*':
        return first * second
    return None


def main():
    splitted = input().split()
    first, second = map(int, splitted[::2])
    operation = splitted[1]
    print(apply(operation, first, second))


if __name__ == "__main__":
    main()
