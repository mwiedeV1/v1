<?php

$t0 = microtime (true);

$a = 0;
$i = 0;
while ($i<10000000.00001) {
  $a+=10.000001;
  $i++;
}


print (microtime(true)-$t0." sec\r\n");

print ($a."\r\n");
?>
