def main():
    print 'START...'
    while True:
        print '>>>',
        string = raw_input()
        print string
        if string == 'error':
            print 'ERROR!'
            main()

if __name__ == '__main__':
    main()