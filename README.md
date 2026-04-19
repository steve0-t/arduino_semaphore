# Wiring

**Requirements:**
  - Arduino UNO
  - 4 cables
  - Breadboard (5 columns is enough)
  - 3 LEDs - 1 red, 1 yellow, 1 green
  - 3 LEDs - 3 **220 Ohm** resistors

**Connect cable from:**
  - A1 to red LED
  - A2 to yellow LED
  - A3 to green LED

Propagate current from LEDs through **220 Ohm** resistors to ground rail


# Build/upload steps

Use the scripts in the scripts directory

**run_monitor.sh** - allows user to type in a command

**upload.sh** - builds and uploads code to the board

**run_native.sh** - allows user to run the program even without board connected

**test_native.sh** - runs tests on native platform (i.e. user's computer)

**WARNING - DO NOT RUN _upload.sh_ WHILE _run_monitor.sh_ IS RUNNING**

Provided _platformio.io_ file specifies 3 environments (uno, native_run, native_test).

# Supported commands

In a form ```<cmd type="<type>" state="<state>"/>```

```<type>``` is one of:
  - set
  - get
  - ping (this one is for fun)

```<state>``` is one of:
  - STOP
  - READY
  - GO
  - CAUTION
  - OFF

# Known limitations

Commands are case sensitive

Only self closing tags are supported so far
