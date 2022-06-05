from umbelt import Umbelt


def main():
    umb = Umbelt(name="Umlaut")

    if umb.connect():
        umb.send_command('MU')

if __name__ == "__main__":
    main()
