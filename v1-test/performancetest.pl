 use Time::HiRes qw( usleep ualarm gettimeofday tv_interval nanosleep
		      clock_gettime clock_getres clock_nanosleep clock
                      stat);


 $t0 = [gettimeofday];
$i = 0;
$a = 0;
while ($i<1000000.000001){
  $a+=10.000001;
  $i++;
}

  ($seconds, $microseconds) = gettimeofday;
  $elapsed = tv_interval ( $t0, [$seconds, $microseconds]);
print ($elapsed);
