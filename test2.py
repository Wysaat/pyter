def raw_read(self):
    digits = '0123456789'
    self.indentation = 0
    if self.eob:
        self.dedents += len(self.indentation_stack) - 1
        self.indentation_stack = [0]
        self.eob = 0
    if self.dedents > 0:
        self.dedents -= 1
        self.items.append(DEDENT)
        return DEDENT
    if self.eof and self.string == '':
        return EOF
    if self.index < len(self.string):
        if self.string[self.index:].strip() == '':
            item = ''
            self.index = len(self.string)
            self.items.append(item)
            self.eol = 1
        else:
            if self.index == 0:
                while self.string[self.index] in [' ', '\t',]:
                    self.indentation += 1
                    self.index += 1
                if self.indentation > self.indentation_stack[-1]:
                    self.indentation_stack.append(self.indentation)
                    self.items.append(INDENT)
                    return INDENT
                elif self.indentation < self.indentation_stack[-1]:
                    for i in range(len(self.indentation_stack)-1, -1, -1):
                        if self.indentation > self.indentation_stack[i]:
                            la.error('indentation_error: unindent does not match any outer indentation level')
                        elif self.indentation == self.indentation_stack[i]:
                            self.dedents -= 1
                            self.items.append(DEDENT)
                            return DEDENT
                        self.dedents += 1
                        self.indentation_stack.pop()
            else:
                while self.string[self.index] in [' ', '\t',]:
                    self.index += 1
            if self.index+2 < len(self.string) and self.string[self.index:self.index+3] in tokens:
                item = self.string[self.index:self.index+3]
                self.index += 3
                self.items.append(item)
            elif self.index+1 < len(self.string) and self.string[self.index:self.index+2] in tokens:
                item = self.string[self.index:self.index+2]
                self.index += 2
                self.items.append(item)
            elif self.index < len(self.string) and self.string[self.index] in tokens:
                item = self.string[self.index]
                self.index += 1
                self.items.append(item)
            elif self.string[self.index] is '.':
                if self.index+1 < len(self.string) and self.string[self.index+1] in digits:
                    item = self.read_numeric_literal()
                else:
                    item = self.string[self.index]
                    self.index += 1
                    self.items.append(item)
            elif self.string[self.index] in digits:
                item = self.read_numeric_literal()
            elif self.string[self.index] in ["'", '"']:
                item = self.read_string_literal()
            else:
                item = ''
                while self.index < len(self.string) and self.string[self.index] in letters + digits + '_':
                    item += self.string[self.index]
                    self.index += 1
                if self.index == len(self.string):
                    self.items.append(item)
                elif item not in stringprefixes:
                    self.items.append(item)
                elif item in stringprefixes and self.string[self.index] not in ["'", '"']:
                    self.items.append(item)
                else:
                    item = self.read_string_literal(item)
        return item
    else:
        item = ''
        self.items.append(item)
        self.eol = 1
        return item



