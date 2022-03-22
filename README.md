# pop

A dead simple note/command playback queue. You give it a line of input, it gives you the next line of a file. Written for live music performances involving pulse signals, but it is fully plaintext and unixy, so use it as you please.

## Build

    make && make install

Tested on MacOS and OpenBSD. Don't care about anything else. Hack it accordingly if this doesn't work.

## Usage

    pop <score data files ...>

### Basic

Once called with at least one file as its argument, `pop` will wait for stdin input. When it receives any signal, including a blank line, it will spit one line of the file to stdout, where it can be processed accordingly. When the file returns EOF, `pop` will rewind and read it from the start once more. The loop has to be broken with ^c.

It mostly makes sense to run `pop` with pipes and fifos.

### Advanced

`pop` can switch between multiple files: up to 32—edit code if you need more. When the `s<n>` command is issued, where `<n>` is the file arg number, `pop` will begin reading from arg `<n>` after it has read through all of its current file. A `s` command is special input that will not advance the current file by a line. This "on deck" behavior is geared towards live music performance, much like selecting the next pattern on a drum machine. If you issue another `s` command before the file has changed, it will overwrite the other queued number.

It can also switch with a capital `S<n>` command. This will jump immediately to a new file; the next input will yield the first line of file `<n>`. One can rewind a file early by providing an `<n>` that is the same number as the current file. This is a good way to create irregular rhythms.

`pop` will take any input, but if you'd like it to listen for special signals, place it behind `pop-shield <key>` in the pipe, where `<key>` is a single character that all commands must be prefixed by to register with `pop`. The composer can read separate input signals (such as the high/low tom pulses on a TR-606), tag them with separate keys, and then aggregate them into a single stream with fifos, `tee`, and `pop-shield`, ensuring that different pulses trigger different scores.

## Examples

### Drum machine sync

Here I have a TR-606's sync pulse plugged into my microphone socket, where [bang](https://github.com/jim1989/bang) is listening. When it encounters the pulse it pipes an "a" into `pop`, which then fires off a note into a FIFO where my synthesizer [boar](https://github.com/jimd1989/boar) is waiting for input. `boar`'s parameters can be edited in real time by echoing more commands to the same FIFO. Analog and digital in harmony!

The `semicolons` command allows multiple notes to be placed on the same line of the score, which is pretty important for this mode of composition. It is found in [boar-extras](https://github.com/jimd1989/boar-extras).

    mkfifo performance
    boar <> performance
    bang 142 12 a | pop score | semicolons > performance
    echo "L 0.5" > performance
    echo "P 2.0" > performance
    ...

### Shielding and switching

First make some fifos to read/write commands.

    mkfifo /tmp/input-fifo
    mkfifo /tmp/output-fifo

Then read from `input-fifo`, splitting the stream to `output-fifo` and a `pop` instance. Nothing should happen because fifos are blocking.

    cat /tmp/input-fifo | tee /tmp/output-fifo | pop-shield b | pop examples/stooges

After that, open a new terminal and feed `output-fifo` into another `pop` command. Once again, nothing will happen.

    cat /tmp/output-fifo | pop-shield a | pop examples/numbers examples/letters

Open a third terminal and use `cat` to create a repl for both scores.

    rlwrap cat > /tmp/input-fifo

One `pop` command is shielded behind a key "a", and the other behind "b". In this repl session, simply pressing enter will not trigger either file, where it would if they were unshielded.

If you enter

    a

however, you should see an output of "1" in one of the terminal windows, but nothing from the other.

Likewise, typing

    b

will print "lenny" in the other terminal window, but not touch the one that prints numbers.

Typing

    aS2
    a

will switch terminal a immediately to letters, and print "a".

But

    bS2
    b

will warn you that no second file is available in terminal 2, and simply print "curly".

Type

    as1
    a

and take note of the lowercase `s`. "b" will be printed out in terminal a, because file 2 is still being read. Terminal a will only switch back to file 1 when all of file 2 is done. Hit `a` multiple times to confirm this.

Replace the `cat` shell with a dynamic stream of events from an external instrument, an algorithm, a multi-user chat where each participant is assigned a key, etc, and you can see how flexible this pattern is. This may be verbose to set up, but it is robust and theoretically simple. As you compose music, you should build up your piece in a project directory as if it were code. Write a script in the language of your choice that automates all this signal routing. 

### Clock divider

`/examples/divider` looks like the following

```



a
```

That is, 3 blank lines, then an "a".

If this score is fed a signal unshielded, but it is piped into `pop-shield a`, it will only trigger its score on every fourth beat.

     pop examples/divider | pop-shield a | pop examples/stooges

Just hit enter a bunch, and you'll only see a name printed after four beats.

## See also

My instruments and composition tools work with plaintext by design, but you can translate from audio pulses or MIDI with

- [bang](https://github.com/jimd1989/bang) (needs work)
- [boar-midi](https://github.com/jimd1989/boar-midi)
- [boar-extras](https://github.com/jimd1989/boar-extras)

Think simple, think big!
