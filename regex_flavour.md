## Syntax

1. Syntax Elements

    + `\`        Escape (Enables or disables metachar)
    + `|`        Alternation
    + `(...)`    Group
    + `[...]`    Character Class
    + `{{...}}`  Rule embed

2. Characters

    + `\t`           Horizontal Tab         (0x09)
    + `\v`           Vertical Tab           (0x0b)
    + `\n`           Newline (LF)           (0x0a)
    + `\r`           Carriage Return        (0x0d)
    + `\b`           Backspace              (0x08)
    + `\f`           Form Feed              (0x0c)
    + `\a`           Bell                   (0x07)
    + `\uHHHHHHHH`   Wide Unicode codepoint (Code point value)

3. Character Types

    1. Classes

        + `.`            Any character except newline
        + `\w`           Word character                (Letter|Mark|Number|Connector Punc)
        + `\W`           non-word character            (reverse of `\w`)
        + `\s`           Whitespace character          (Line Sep | Par Sep | Space Sep)
        + `\S`           non-whitespace character      (reverse of `\s`)
        + `\d`           Decimal digit character       (Number Decimal)
        + `\D`           non-digit characte            (reverse of `\d`)

    2. Unicode Classes or properties

        + `\p{property-name}`   Unicode Class (L|Z|M|N|P) and their subcategories
    
4. Quantifiers

    1. Greedy

        + `?`        1 or 0 times
        + `*`        0 or more times
        + `+`        1 or more times
        + `{m,n}`    (n <= m) at least n but no more than m times
        + `{n,}`     at least n times
        + `{n}`      n times
    
    2. Reluctant

        + `*?`       0 or more times
        + `+?`       1 or more times 
    

5. Assertions (not yet implemented)

    + `^`      beginning of line 
    + `$`      end of line
    + `\b`     word boundary
    + `\B`     non-word boundary
    + `\A`     True beginning of string
    + `\G`     beginning of match
    + `\K`     start next match here

6. Character Classes (Note: Alternation of character classes merges them together)

    + `^...`    negative character class

7. Extended Groups (not yet implemented)

    + `(?:...)` Shy group (non-capturing)
    + `(?imIM:subexp)`   specify option for match of subexp (not a group)
        + `i`: ignore case
        + `m`: multiline
        + `I`: match case
        + `M`: non multiline

8. Backreferences (not yet implemented)

    + Explanation: Backreferencing a group means to re-match its actual matched value    
    + `\<n>`    backreference group number `n`

9. Subexpr calls (not yet implemented)

    + Explanation: Subexpr calling attempts to match a given group (not recursive) *again*, and then continuing from the ending position (if the match was a success) or terminating and reverting to the closest valid match
    + `\g<n>`   attemt to call the `n`th group (leftmost and recursive calls are not allowed)

10. Capture groups (not yet implemented)

    + `(...)` is a capture group and its index is the number of open-parens since the beginning of the regexp (unique)


## Basic Features

| support         | '+' quantifier | Nested character classes | Non-greedy quantifiers | Non-capturing groups | Recursion      | Lookahead      | Lookbehind     | Backreferences | Indexable captures | Directives         | Conditionals       | Atomic Groups      | Named Captures     | Comments           | Embedded code      | Unicode Property   | Balancing Groups   | Variable length lookbehind |
| :-------------  | :------------- | :-------------           | :-------------         | :-------------       | :------------- | :------------- | :------------- | :------------- | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------     | :-------------             |
| Current support | Yes            | No                       | Yes                    | No                   | No             | No             | No             | No             | No                 | No                 | No                 | Yes                | No                 | No                 | No                 | Partial            | No                 | No                         |
| Planned support | -              | No                       | -                      | Yes                  | No             | Yes            | Yes            | Yes            | Yes                | Yes                | No                 | -                  | No                 | No                 | ?                  | Yes                | No                 | No                         |


## API features

| support | UTF-16  | UTF-8   | Multiline | Partial match               |
| :------ | :------ | :------ | :------   | :------                     |
| Current | No      | Yes     | Yes       | Only if partial rule exists |
| Planned | No      | -       | -         | -                           |