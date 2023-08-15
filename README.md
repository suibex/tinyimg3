# tinyimg3
tinyimg3 is a tiny ARM Versatile Platform Baseboard kernel that uses firmware that is something like IMG3 format.
<h1>Motivation</h1>
<p>I always loved doing low-level programming, hardware programming...</p>
<p>Also i've been always kind of intrigued of the iPhone 2G design choices, and OS-dev.</p>

<h1>IMG3 File format</h1>

https://www.theiphonewiki.com/wiki/IMG3_File_Format
<br>

<h1>What does this kernel do?</h1>
<p>First, it downloads "custom" firmware from UART0 and writes it to NOR flash, then it uses PL110 CLCD component to display a b&w gradient on the screen.</p>
<img src="https://github.com/suibex/tinyimg3/assets/59802817/98ed4902-4eda-4370-b260-4e5fc8500783">

