from ble_host import UmbeltBoard


def main():
    umb = UmbeltBoard(name="Umlaut")

    if umb.connect():
        umb.send_command('MU')

if __name__ == "__main__":
    main()
