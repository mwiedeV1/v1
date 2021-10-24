<?php

date_default_timezone_set ("Europe/Berlin");

define ('PI',  3.14159265359);
define ('DEG', PI/180.0);
define ('RAD', 180.0/PI);
define ('PI2',  2*PI);
$t0 = microtime (true);

function sqr($x)
{
  return $x*$x;
}


// return integer $value, closer to 0
function Int($x)
{

  if ($x<0) { return(ceil($x)); } else return(floor($x));
}

function frac($x) { return($x-floor($x)); }

function Mod($a, $b) { return($a-floor($a/$b)*$b); }


// Modulo PI
function Mod2Pi($x)
{
  $x = Mod($x, 2*PI);
  return($x);
}


function round100000($x) { return(round(100000*$x)/100000); }
function round10000($x) { return(round(10000*$x)/10000); }
function round1000($x) { return(round(1000*$x)/1000); }
function round100($x) { return(round(100*$x)/100); }
function round10($x) { return(round(10*$x)/10); }


$emptyStr = "--";

function HHMM($hh) 
{
	global $emptyStr;
  if ($hh==0) return($emptyStr);
  
  $m = frac($hh)*60.0;
  $h = Int($hh);
  if ($m>=59.5) { $h++; $m -=60.0; }
  $m = round($m);
	$hStr = sprintf ("%02u:%02u = %s", $h, $m, round1000($hh));
	return $hStr;
  // return($hStr+" = "+round1000($hh));
}

function HHMMSS($hh) 
{
  if ($hh==0) return($emptyStr);
  
  $m = frac($hh)*60;
  $h = Int($hh);
  $s = frac($m)*60;
  $m = Int($m);
  if ($s>=59.5) { $m++; $s -=60.0; }
  if ($m>=60)   { $h++; $m -=60; }
  $s = round($s);
	$hStr = sprintf ("%02u:%02u:%02u = %s", $h, $m, $s, round10000($hh));
	return $hStr;
//  return($hStr+" = "+round10000($hh));
}


function Sign($lon)
{ 
	$signs= array ("Widder", "Stier", "Zwillinge", "Krebs", "Loewe", "Jungfrau", 
   "Waage", "Skorpion", "Schuetze", "Steinbock", "Wassermann", "Fische");
  return( $signs[floor($lon*RAD/30)] );
}


// Calculate Julian date: valid only from 1.3.1901 to 28.2.2100
function CalcJD($day,$month,$year)
{
  $jd = 2415020.5-64; // 1.1.1900 - correction of algorithm

  if ($month<=2) { $year--; $month += 12; }
  $jd += Int( ($year-1900)*365.25 );
  $jd += Int( 30.6001*(1+$month) );
	return($jd + $day);
}


// Julian Date to Greenwich Mean Sidereal $time
function gmst($jd)
{
  $UT = frac($jd-0.5)*24.0; // $UT in hours
  $jd = floor($jd-0.5)+0.5;   // $jd at 0 hours $UT
  $T = ($jd-2451545.0)/36525.0;
  $T0 = 6.697374558 + $T*(2400.051336 + $T*0.000025862);
  return(Mod($T0+$UT*1.002737909, 24.0));
}


// Convert Greenweek mean sidereal $time to $UT
function GMST2UT($jd, $gmst)
{
  $jd = floor($jd-0.5)+0.5;   // $jd at 0 hours $UT
  $T = ($jd-2451545.0)/36525.0;
  $T0 = Mod(6.697374558 + $T*(2400.051336 + $T*0.000025862), 24.0);
  //var $UT = 0.9972695663*Mod(($gmst-$T0), 24.);
  $UT = 0.9972695663*(($gmst-$T0));
  return($UT);
}


// Local Mean Sidereal $time, geographical longitude in radians, East is positive
function GMST2LMST($gmst, $lon)
{
  $lmst = Mod($gmst+RAD*$lon/15, 24.0);
  return( $lmst );
}

// Transform ecliptical coordinates ($lon/$lat) to equatorial coordinates (RA/dec)
function Ecl2Equ($coor, $TDT)
{
  $T = ($TDT-2451545.0)/36525.0; // Epoch 2000 January 1.5
  $eps = (23.0+(26+21.45/60.0)/60.0 + $T*(-46.815 +$T*(-0.0006 + $T*0.00181) )/3600.0 )*DEG;
  $coseps = cos($eps);
  $sineps = sin($eps);
  
  $sinlon = sin($coor["LON"]);
  $coor["ra"]  = Mod2Pi( atan2( ($sinlon*$coseps-tan($coor["LAT"])*$sineps), cos($coor["LON"]) ) );
  $coor["dec"] = asin( sin($coor["LAT"])*$coseps + cos($coor["LAT"])*$sineps*$sinlon );
  
  return $coor;
}

// Transform equatorial coordinates (RA/Dec) to horizonal coordinates (azimuth/$altitude)
// Refraction is ignored
function Equ2Altaz($coor, $TDT, $geolat, $lmst)
{
  $cosdec = cos($coor["dec"]);
  $sindec = sin($coor["dec"]);
  $lha = $lmst - $coor["ra"];
  $coslha = cos($lha);
  $sinlha = sin($lha);
  $coslat = cos($geolat);
  $sinlat = sin($geolat);
  
  $N = -$cosdec * $sinlha;
  $D = $sindec * $coslat - $cosdec * $coslha * $sinlat;
  $coor["az"] = Mod2Pi( atan2($N, $D) );
  $coor["alt"] = asin( $sindec * $sinlat + $cosdec * $coslha * $coslat );

  return $coor;
}


// Transform geocentric equatorial coordinates (RA/Dec) to topocentric equatorial coordinates
function GeoEqu2TopoEqu($coor, $observer, $lmst)
{
  $cosdec = cos($coor["dec"]);
  $sindec = sin($coor["dec"]);
  $coslst = cos($lmst);
  $sinlst = sin($lmst);
  $coslat = cos($observer["LAT"]); // we should use geocentric latitude, not geodetic latitude
  $sinlat = sin($observer["LAT"]);
  $rho = $observer["radius"]; // $observer-geocenter in Kilometer
  
  $x = $coor["distance"]*$cosdec*cos($coor["ra"]) - $rho*$coslat*$coslst;
  $y = $coor["distance"]*$cosdec*sin($coor["ra"]) - $rho*$coslat*$sinlst;
  $z = $coor["distance"]*$sindec - $rho*$sinlat;

  $coor["distanceTopocentric"] = sqrt($x*$x + $y*$y + $z*$z);
  $coor["decTopocentric"] = asin($z/$coor["distanceTopocentric"]);
  $coor["raTopocentric"] = Mod2Pi( atan2($y, $x) );

  return $coor;
}


// Calculate cartesian from polar coordinates
function EquPolar2Cart( $lon, $lat, $distance )
{
  $cart = array ();
  $rcd = cos($lat)*$distance;
  $cart["x"]= $rcd*cos($lon);
  $cart["y"] = $rcd*sin($lon);
  $cart["z"] = $distance * sin($lat);
  return($cart);
}


// Calculate observers cartesian equatorial coordinates ($x,$y,$z in celestial frame) 
// from geodetic coordinates (longitude, latitude, height above WGS84 ellipsoid)
// Currently only used to calculate distance of $a body from the $observer
function Observer2EquCart( $lon, $lat, $height, $gmst )
{
  $flat = 298.257223563;        // WGS84 flatening of earth
  $aearth = 6378.137;           // GRS80/WGS84 semi major axis of earth ellipsoid
    $cart = array ();
  // Calculate geocentric latitude from geodetic latitude
  $co = cos ($lat);
  $si = sin ($lat);
  $fl = 1.0 - 1.0 / $flat;
  $fl = $fl * $fl;
  $si = $si * $si;
  $u = 1.0 / sqrt ($co * $co + $fl * $si);
  $a = $aearth * $u + $height;
  $b = $aearth * $fl * $u + $height;
  $radius = sqrt ($a * $a * $co * $co + $b * $b * $si); // geocentric distance from earth center
  $cart["y"] = acos ($a * $co / $radius); // geocentric latitude, rad
  $cart["x"] = $lon; // longitude stays the same
  if ($lat < 0.0) { $cart["y"] = -$cart["y"]; } // adjust sign
  $cart = EquPolar2Cart( $cart["x"], $cart["y"], $radius ); // convert from geocentric polar to geocentric cartesian, with regard to Greenwich
  // rotate around earth'$s polar axis to align coordinate system from Greenwich to vernal equinox
  $x=$cart["x"]; $y=$cart["y"];
  $rotangle = $gmst/24*2*PI; // sideral $time $gmst given in hours. Convert to radians
  $cart["x"] = $x*cos($rotangle)-$y*sin($rotangle);
  $cart["y"] = $x*sin($rotangle)+$y*cos($rotangle);
  $cart["radius"] = $radius;
  $cart["LON"] = $lon;
  $cart["LAT"] = $lat;
  return($cart);
}


// Calculate coordinates for Sun
// Coordinates are accurate to about 10s (right ascension) 
// and $a few minutes of arc (declination)
function SunPosition($TDT, $geolat=null, $lmst=null)
{
  $D = $TDT-2447891.5;
  
  $eg = 279.403303*DEG;
  $wg = 282.768422*DEG;
  $e  = 0.016713;
  $a  = 149598500; // km
  $diameter0 = 0.533128*DEG; // angular diameter of Moon at $a distance
  
  $MSun = 360*DEG/365.242191*$D+$eg-$wg;
  $nu = $MSun + 360.0*DEG/PI*$e*sin($MSun);
  
  $sunCoor = array ();
  $sunCoor["LON"] =  Mod2Pi($nu+$wg);
  $sunCoor["LAT"] = 0;
  $sunCoor["anomalyMean"] = $MSun;
  
  $sunCoor["distance"] = (1-sqr($e))/(1+$e*cos($nu)); // distance in astronomical units
  $sunCoor["diameter"] = $diameter0/$sunCoor["distance"]; // angular diameter in radians
  $sunCoor["distance"] *= $a;                         // distance in km
  $sunCoor["parallax"] = 6378.137/$sunCoor["distance"];  // horizonal parallax

  $sunCoor = Ecl2Equ($sunCoor, $TDT);
  
  // Calculate horizonal coordinates of sun, if geographic positions is given
  if ($geolat!=null && $lmst!=null) {
    $sunCoor = Equ2Altaz($sunCoor, $TDT, $geolat, $lmst);
  }
  
  $sunCoor["sign"] = Sign($sunCoor["LON"]);
  return $sunCoor;
}


// Calculate data and coordinates for the Moon
// Coordinates are accurate to about 1/5 degree (in ecliptic coordinates)
function MoonPosition($sunCoor, $TDT, $observer=null, $lmst=null)
{
  $D = $TDT-2447891.5;
  
  // Mean Moon orbit elements as of 1990.0
  $l0 = 318.351648*DEG;
  $P0 =  36.340410*DEG;
  $N0 = 318.510107*DEG;
  $i  = 5.145396*DEG;
  $e  = 0.054900;
  $a  = 384401; // km
  $diameter0 = 0.5181*DEG; // angular diameter of Moon at $a distance
  $parallax0 = 0.9507*DEG; // parallax at distance $a
  
  $l = 13.1763966*DEG*$D+$l0;
  $MMoon = $l-0.1114041*DEG*$D-$P0; // Moon'$s mean anomaly $m
  $N = $N0-0.0529539*DEG*$D;       // Moon'$s mean ascending node longitude
  $C = $l-$sunCoor["LON"];
  $Ev = 1.2739*DEG*sin(2*$C-$MMoon);
  $Ae = 0.1858*DEG*sin($sunCoor["anomalyMean"]);
  $A3 = 0.37*DEG*sin($sunCoor["anomalyMean"]);
  $MMoon2 = $MMoon+$Ev-$Ae-$A3;  // corrected Moon anomaly
  $Ec = 6.2886*DEG*sin($MMoon2);  // equation of centre
  $A4 = 0.214*DEG*sin(2*$MMoon2);
  $l2 = $l+$Ev+$Ec-$Ae+$A4; // corrected Moon'$s longitude
  $V = 0.6583*DEG*sin(2*($l2-$sunCoor["LON"]));
  $l3 = $l2+$V; // true orbital longitude;

  $N2 = $N-0.16*DEG*sin($sunCoor["anomalyMean"]);
  
  $moonCoor = array ();
  $moonCoor["LON"] = Mod2Pi( $N2 + atan2( sin($l3-$N2)*cos($i), cos($l3-$N2) ) );
  $moonCoor["LAT"] = asin( sin($l3-$N2)*sin($i) );
  $moonCoor["orbitLon"] = $l3;
  
  $moonCoor = Ecl2Equ($moonCoor, $TDT);
  // relative distance to semi mayor axis of lunar oribt
  $moonCoor["distance"] = (1-sqr($e)) / (1+$e*cos($MMoon2+$Ec) );
  $moonCoor["diameter"] = $diameter0/$moonCoor["distance"]; // angular diameter in radians
  $moonCoor["parallax"] = $parallax0/$moonCoor["distance"]; // horizontal parallax in radians
  $moonCoor["distance"] *= $a; // distance in km

  // Calculate horizonal coordinates of sun, if geographic positions is given
  if ($observer!=null && $lmst!=null) {
  	
    // transform geocentric coordinates into topocentric (==$observer based) coordinates
		$moonCoor = GeoEqu2TopoEqu($moonCoor, $observer, $lmst);
		$moonCoor["raGeocentric"] = $moonCoor["ra"]; // backup geocentric coordinates		
		$moonCoor["decGeocentric"] = $moonCoor["dec"];
		$moonCoor["ra"]=$moonCoor["raTopocentric"];
		$moonCoor["dec"]=$moonCoor["decTopocentric"];
    $moonCoor = Equ2Altaz($moonCoor, $TDT, $observer["LAT"], $lmst); // now ra and dec are topocentric
  }
  
  // Age of Moon in radians since New Moon (0) - Full Moon (pi)
  $moonCoor["moonAge"] = Mod2Pi($l3-$sunCoor["LON"]);   
  $moonCoor["phase"]   = 0.5*(1-cos($moonCoor["moonAge"])); // Moon phase, 0-1
  
  $phases = array ("Neumond", "Zunehmende Sichel", "Erstes Viertel", "Zunehmender Mond", 
   "Vollmond", "Abnehmender Mond", "Letztes Viertel", "Abnehmende Sichel", "Neumond");
  $mainPhase = 1.0/29.53*360*DEG; // show 'Newmoon, 'Quarter' for +/-1 day arond the actual event
  $P = Mod($moonCoor["moonAge"], 90.0*DEG);
  if ($P < $mainPhase || $P > 90*DEG-$mainPhase) $P = 2*round($moonCoor["moonAge"] / (90.0*DEG));
  else $P = 2*floor($moonCoor["moonAge"] / (90.0*DEG))+1;
  $moonCoor["moonPhase"] = $phases[$P];
  
  $moonCoor["sign"] = Sign($moonCoor["LON"]);

  return($moonCoor);
}


// Input true $altitude in radians, Output: increase in $altitude in degrees
function Refraction($alt)
{
  $altdeg = $alt*RAD;
  if ($altdeg<-2 || $altdeg>=90) return(0);
   
  $pressure    = 1015;
  $temperature = 10;
  if ($altdeg>15) return( 0.00452*$pressure/( (273+$temperature)*tan($alt)) );
  
  $y = $alt;
  $D = 0.0;
  $P = ($pressure-80)/930;
  $Q = 0.0048*($temperature-10);
  $y0 = $y;
  $D0 = $D;

  for ($i=0; $i<3; $i++) {
	  $N = $y+(7.31/($y+4.4));
	  $N = 1.0/tan($N*DEG);
	  $D = $N*$P/(60.0+$Q*($N+39.0));
	  $N = $y-$y0;
	  $y0 = $D-$D0-$N;
	  if (($N != 0) && ($y0 != 0)) { $N = $y-$N*($alt+$D-$y)/$y0; }
	  else { $N = $alt+$D; }
	  $y0 = $y;
	  $D0 = $D;
	  $y = $N;
  }
  return( $D ); // Hebung durch Refraktion in radians
}


// returns Greenwich sidereal $time (hours) of $time of $rise 
// and set of object with coordinates $coor.ra/$coor.dec
// at geographic position $lon/$lat (all values in radians)
// Correction for refraction and semi-diameter/parallax of body is taken care of in function $riseset
// $h is used to calculate the twilights. It gives the required elevation of the disk center of the sun
function GMSTRiseSet($coor, $lon, $lat, $h=null)
{
  $h = ($h == null) ? 0 : $h; // set default $value
  $riseset = array();
//  tagbogen = acos(-tan($lat)*tan($coor.dec)); // simple formula if twilight is not required
  $tagbogen = acos((sin($h) - sin($lat)*sin($coor["dec"])) / (cos($lat)*cos($coor["dec"])));

  $riseset["transit"] =      RAD /15*(          $coor["ra"]-$lon);
  $riseset["rise"]    = 24 + RAD /15*(-$tagbogen+$coor["ra"]-$lon); // calculate $gmst of $rise of object
  $riseset["set"]     =      RAD /15*( $tagbogen+$coor["ra"]-$lon); // calculate $gmst of set of object

  // using the modulo function Mod, the day number goes missing. This may get $a problem for the moon
  $riseset["transit"] = Mod($riseset["transit"], 24);
  $riseset["rise"]    = Mod($riseset["rise"], 24);
  $riseset["set"]     = Mod($riseset["set"], 24);

  return($riseset);
}



// Find $gmst of $rise/set of object from the two calculates 
// (start)points (day 1 and 2) and at midnight $UT(0)
function InterpolateGMST($gmst0, $gmst1, $gmst2, $timefactor)
{
  return( ($timefactor*24.07*$gmst1- $gmst0*($gmst2-$gmst1)) / ($timefactor*24.07+$gmst1-$gmst2) );
}


// $jd is the Julian Date of 0h UTC $time (midnight)
function riseset($jd0UT, $coor1, $coor2, $lon, $lat, $timeinterval, $altitude=null)
{
  // $altitude of sun center: semi-diameter, horizontal parallax and (standard) refraction of 34'
  $alt = 0; // calculate 
  $altitude = ($altitude == null) ? 0 : $altitude; // set default $value

  // true height of sun center for $sunRise and set calculation. Is kept 0 for twilight (ie. $altitude given):
  if (!$altitude) $alt = 0.5*$coor1["diameter"]-$coor1["parallax"]+34/60*DEG; 
  
  $rise1 = GMSTRiseSet($coor1, $lon, $lat, $altitude);
  $rise2 = GMSTRiseSet($coor2, $lon, $lat, $altitude);
  
  $rise = array();
  
  // unwrap $gmst in case we move across 24h -> 0h
  if ($rise1["transit"] > $rise2["transit"] && abs($rise1["transit"]-$rise2["transit"])>18) $rise2["transit"] += 24;
  if ($rise1["rise"]    > $rise2["rise"]    && abs($rise1["rise"]   -$rise2["rise"])>18)    $rise2["rise"] += 24;
  if ($rise1["set"]     > $rise2["set"]     && abs($rise1["set"]    -$rise2["set"])>18)     $rise2["set"]  += 24;
  $T0 = gmst($jd0UT);
  //  $T02 = $T0-$zone*1.002738; // Greenwich sidereal $time at 0h $time $zone ($zone: hours)

  // Greenwich sidereal $time for 0h at selected longitude
  $T02 = $T0-$lon*RAD/15*1.002738; if ($T02 < 0) $T02 += 24; 

  if ($rise1["transit"] < $T02) { $rise1["transit"] += 24; $rise2["transit"] += 24; }
  if ($rise1["rise"]    < $T02) { $rise1["rise"]    += 24; $rise2["rise"]    += 24; }
  if ($rise1["set"]     < $T02) { $rise1["set"]     += 24; $rise2["set"]     += 24; }
  
  // Refraction and Parallax correction
  $decMean = 0.5*($coor1["dec"]+$coor2["dec"]);
  $psi = acos(sin($lat)/cos($decMean));
  $y = asin(sin($alt)/sin($psi));
  $dt = 240*RAD*$y/cos($decMean)/3600; // $time correction due to refraction, parallax

  $rise["transit"] = GMST2UT( $jd0UT, InterpolateGMST( $T0, $rise1["transit"], $rise2["transit"], $timeinterval) );
  $rise["rise"]    = GMST2UT( $jd0UT, InterpolateGMST( $T0, $rise1["rise"],    $rise2["rise"],    $timeinterval) -$dt );
  $rise["set"]     = GMST2UT( $jd0UT, InterpolateGMST( $T0, $rise1["set"],     $rise2["set"],     $timeinterval) +$dt );

  return($rise);  
}


// Find (local) $time of $sunRise and sunset, and twilights
// $jd is the Julian Date of 0h local $time (midnight)
// Accurate to about 1-2 minutes
// $recursive: 1 - calculate $rise/set in UTC in $a second run
// $recursive: 0 - find $rise/set on the current local day. This is set when doing the first call to this function
function sunRise($jd, $deltaT, $lon, $lat, $zone, $recursive)
{
  $jd0UT = floor($jd-0.5)+0.5;   // $jd at 0 hours $UT
  $coor1 = SunPosition($jd0UT+  $deltaT/24/3600);
  $coor2 = SunPosition($jd0UT+1+$deltaT/24/3600); // calculations for next day'$s UTC midnight
  
  $risetemp =  array ();
  $rise = array ();
  // $rise/set $time in UTC. 
  $rise = riseset($jd0UT, $coor1, $coor2, $lon, $lat, 1); 
  if (!$recursive) { // check and adjust to have $rise/set $time on local calendar day
    if ($zone>0) {
      // $rise $time was yesterday local $time -> calculate $rise $time for next UTC day
      if ($rise["rise"]>=24-$zone || $rise["transit"]>=24-$zone || $rise["set"]>=24-$zone) {
        $risetemp = $sunRise($jd+1, $deltaT, $lon, $lat, $zone, 1);
        if ($rise["rise"]>=24-$zone) $rise["rise"] = $risetemp["rise"];
        if ($rise["transit"] >=24-$zone) $rise["transit"] = $risetemp["transit"];
        if ($rise["set"] >=24-$zone) $rise["set"]  = $risetemp["set"];
      }
    }
    else if ($zone<0) {
      // $rise $time was yesterday local $time -> calculate $rise $time for next UTC day
      if ($rise["rise"]<-$zone || $rise["transit"]<-$zone || $rise["set"]<-$zone) {
        $risetemp = $sunRise($jd-1, $deltaT, $lon, $lat, $zone, 1);
        if ($rise["rise"]<-$zone) $rise["rise"] = $risetemp["rise"];
        if ($rise["transit"]<-$zone) $rise["transit"] = $risetemp["transit"];
        if ($rise["set"] <-$zone) $rise["set"]  = $risetemp["set"];
      }
    }

    $rise["transit"] = Mod($rise["transit"]+$zone, 24);
    $rise["rise"]    = Mod($rise["rise"]   +$zone, 24);
    $rise["set"]     = Mod($rise["set"]    +$zone, 24);

	// Twilight calculation
	// civil twilight $time in UTC. 
	$risetemp = riseset($jd0UT, $coor1, $coor2, $lon, $lat, 1, -6*DEG);

	$rise["cicilTwilightMorning"] = Mod($risetemp["rise"] +$zone, 24);
	$rise["cicilTwilightEvening"] = Mod($risetemp["set"]  +$zone, 24);

	// nautical twilight $time in UTC. 
	$risetemp = riseset($jd0UT, $coor1, $coor2, $lon, $lat, 1, -12*DEG);
	$rise["nauticalTwilightMorning"] = Mod($risetemp["rise"] +$zone, 24);
	$rise["nauticalTwilightEvening"] = Mod($risetemp["set"]  +$zone, 24);

	// astronomical twilight $time in UTC. 
	$risetemp = riseset($jd0UT, $coor1, $coor2, $lon, $lat, 1, -18*DEG);
	$rise["astronomicalTwilightMorning"] = Mod($risetemp["rise"] +$zone, 24);
	$rise["astronomicalTwilightEvening"] = Mod($risetemp["set"]  +$zone, 24);

  }
  return( $rise );  
}



// Find local $time of $moonRise and moonset
// $jd is the Julian Date of 0h local $time (midnight)
// Accurate to about 5 minutes or better
// $recursive: 1 - calculate $rise/set in UTC
// $recursive: 0 - find $rise/set on the current local day (set could also be first)
// returns '' for $moonRise/set does not occur on selected day
function moonRise($jd, $deltaT, $lon, $lat, $zone, $recursive)
{
  $timeinterval = 0.5;
  
  $jd0UT = floor($jd-0.5)+0.5;   // $jd at 0 hours $UT
  $suncoor1 = SunPosition($jd0UT+ $deltaT/24/3600);
  $coor1 = MoonPosition($suncoor1, $jd0UT+ $deltaT/24/3600);

  $suncoor2 = SunPosition($jd0UT +$timeinterval + $deltaT/24/3600); // calculations for noon
  // calculations for next day'$s midnight
  $coor2 = MoonPosition($suncoor2, $jd0UT +$timeinterval + $deltaT/24/3600); 
  
  $risetemp = array();
  $rise = array();
  
  // $rise/set $time in UTC, $time $zone corrected later.
  // Taking into account refraction, semi-diameter and parallax
  $rise = riseset($jd0UT, $coor1, $coor2, $lon, $lat, $timeinterval); 
  
  if (!$recursive) { // check and adjust to have $rise/set $time on local calendar day
    if ($zone>0) {
      // $recursive call to $moonRise returns events in UTC, $zone ignored
      $risetemp = moonRise($jd-1, $deltaT, $lon, $lat, $zone, 1); 
      
      //alert("yesterday="+$risetemp.transit+"  today="+$rise.transit);

      if ($rise["transit"] >= 24-$zone || $rise["transit"] < -$zone) { // transit $time is tomorrow local $time
        if ($risetemp["transit"] < 24-$zone || $risetemp["transit"] >= 48-$zone) $rise["transit"] = ""; // there is no moontransit today
        else {
		  $rise["transit"]  = $risetemp["transit"];
 	      if ($rise["transit"] >= 24) $rise["transit"]-=24;
		}
      }

      if ($rise["rise"] >= 24-$zone || $rise["rise"] < -$zone) { // $rise $time is tomorrow local $time
        if ($risetemp["rise"] < 24-$zone || $risetemp["rise"] >= 48-$zone) $rise["rise"] = ""; // there is no moontransit today
        else {
		  $rise["rise"]  = $risetemp["rise"];
 	      if ($rise["rise"] >= 24) $rise["rise"]-=24;
		}
      }

      if ($rise["set"] >= 24-$zone || $rise["set"] < -$zone) { // set $time is tomorrow local $time
        if ($risetemp["set"] < 24-$zone || $risetemp["set"] >= 48-$zone) $rise["set"] = ""; // there is no moontransit today
        else {
		  $rise["set"]  = $risetemp["set"];
  	      if ($rise["set"] >= 24) $rise["set"]-=24;
		}
      }

    }
    else if ($zone<0) {
      // $rise/set $time was tomorrow local $time -> calculate $rise $time for former UTC day
      if ($rise["rise"]<-$zone || $rise["set"]<-$zone || $rise["transit"]<-$zone) { 
        $risetemp = $moonRise($jd+1, $deltaT, $lon, $lat, $zone, 1);
        
        if ($rise["rise"] < -$zone) {
          if ($risetemp["rise"] > -$zone) $rise["rise"] = ""; // there is no $moonRise today
          else $rise["rise"] = $risetemp["rise"];
        }
        
        if ($rise["transit"] < -$zone)
        {
          if ($risetemp["transit"] > -$zone)  $rise["transit"] = ""; // there is no moonset today
          else $rise["transit"]  = $risetemp["transit"];
        }
        
        if ($rise["set"] < -$zone)
        {
          if ($risetemp["set"] > -$zone)  $rise["set"] = ""; // there is no moonset today
          else $rise["set"]  = $risetemp["set"];
        }
        
      }
    }
    
    if ($rise["rise"])    $rise["rise"] = Mod($rise["rise"]+$zone, 24);    // correct for $time $zone, if $time is valid
    if ($rise["transit"]) $rise["transit"]  = Mod($rise["transit"] +$zone, 24); // correct for $time $zone, if $time is valid
    if ($rise["set"])     $rise["set"]  = Mod($rise["set"] +$zone, 24);    // correct for $time $zone, if $time is valid
  }
  return( $rise );  
}



function InitDate (&$form)
{
	$time    = time ();
  $form["Hour"]    = date ("H", $time);
  $form["Minute"]  = date ("i", $time);
  $form["Second"]  = date ("s", $time);
  $form["Day"]     = date ("d", $time);
  $form["Month"]   = date ("m", $time);
  $form["Year"]    = date ("Y", $time);
  $form["Zone"]    = 1; // date ("z", $time) / 3600;

}


function Init(&$form)
{    
  global $emptyStr;
  
  InitDate($form);
  $form["DeltaT"]  = 65; // $deltaT - difference among 'earth center' versus 'observered' $time ($TDT-$UT), in seconds

  $form["JD"]      = $emptyStr;
  $form["GMST"]    = $emptyStr;
  $form["LMST"]    = $emptyStr;

	// Position FüGermany
  $form["LON"]     = 10.9518465;
  $form["LAT"]     = 48.9174128;

  $form["SunLon"]  = $emptyStr; 
  $form["SunRA"]   = $emptyStr;
  $form["SunDec"]  = $emptyStr;
  $form["SunAz"]   = $emptyStr;
  $form["SunAlt"]  = $emptyStr;
  $form["SunDistance"] = $emptyStr;        
  $form["SunDistanceObserver"] = $emptyStr;        
  $form["SunDiameter"] = $emptyStr;    
  $form["SunSign"] = $emptyStr;    
  $form["SunTransit"] = $emptyStr;    
  $form["sunRise"] = $emptyStr;    
  $form["SunSet"]  = $emptyStr;    
  $form["SunCivilTwilightMorning"] = $emptyStr;    
  $form["SunCivilTwilightEvening"]  = $emptyStr;    
  $form["SunNauticalTwilightMorning"] = $emptyStr;	
  $form["SunNauticalTwilightEvening"]  = $emptyStr;	 
  $form["SunAstronomicalTwilightMorning"] = $emptyStr;	
  $form["SunAstronomicalTwilightEvening"]  = $emptyStr;    

  $form["MoonLon"] = $emptyStr;        
  $form["MoonLat"] = $emptyStr;        
  $form["MoonRA"]  = $emptyStr;        
  $form["MoonDec"] = $emptyStr;        
  $form["MoonAz"]  = $emptyStr;        
  $form["MoonAlt"]  = $emptyStr;        
  $form["MoonDistance"] = $emptyStr;        
  $form["MoonDistanceObserver"] = $emptyStr;        
  $form["MoonDiameter"] = $emptyStr;        
  $form["MoonPhase"]  = $emptyStr;     
  $form["MoonAge"]    = $emptyStr;     
  $form["MoonSign"]   = $emptyStr;
  $form["MoonTransit"] = $emptyStr;   
  $form["moonRise"]   = $emptyStr;   
  $form["MoonSet"]    = $emptyStr;
    
}


function Compute(&$form)
{

  if ($form["Year"]<=1900 || $form["Year"]>=2100 ) {
    die ("Dies Script erlaubt nur Berechnungen von 1900 - 2100");
    return;
  }

  $JD0 = CalcJD( $form["Day"], $form["Month"], $form["Year"] );

  $jd  = $JD0 + (( $form["Hour"] - $form["Zone"]) + $form["Minute"]/60 + ($form["Second"]/3600)) /24;
  $TDT = $jd  + $form["DeltaT"]/24/3600;

  $lat      = $form["LAT"]*DEG; // geodetic latitude of $observer on WGS84
  $lon      = $form["LON"]*DEG; // latitude of $observer
  $height   = 0 * 0.001; // altiude of $observer in meters above WGS84 ellipsoid (and converted to kilometers)

  $gmst = gmst($jd);
  $lmst = GMST2LMST($gmst, $lon);

  $observerCart = Observer2EquCart($lon, $lat, $height, $gmst); // geocentric cartesian coordinates of $observer

  $sunCoor  = SunPosition($TDT, $lat, $lmst*15*DEG);   // Calculate data for the Sun at given $time

  $moonCoor = MoonPosition($sunCoor, $TDT, $observerCart, $lmst*15*DEG);    // Calculate data for the Moon at given $time

  $form["JD"] = round100000($jd);
  $form["GMST"] = HHMMSS($gmst);
  $form["LMST"] = HHMMSS($lmst);

	
  if ($form["Minute"]<10) $form["Minute"] = "0"+$form["Minute"];
  if ($form["Month"]<10) $form["Month"] = "0"+$form["Month"];

  $form["SunLon"]  = round1000($sunCoor["LON"]*RAD);
  $form["SunRA"]   = HHMM($sunCoor["ra"]*RAD/15);
  $form["SunDec"]  = round1000($sunCoor["dec"]*RAD);
  $form["SunAz"]   = round100($sunCoor["az"]*RAD);
  $form["SunAlt"]  = round10($sunCoor["alt"]*RAD+Refraction($sunCoor["alt"]));  // including refraction

  $form["SunSign"] = $sunCoor["sign"];
  $form["SunDiameter"] = round100($sunCoor["diameter"]*RAD*60); // angular diameter in arc seconds
  $form["SunDistance"] = round10($sunCoor["distance"]);

  // Calculate distance from the $observer (on the surface of earth) to the center of the sun
  $sunCart      = EquPolar2Cart($sunCoor["ra"], $sunCoor["dec"], $sunCoor["distance"]);
  $form["SunDistanceObserver"] = round10( sqrt( sqr($sunCart["x"]-$observerCart["x"]) + sqr($sunCart["y"]-$observerCart["y"]) + sqr($sunCart["z"]-$observerCart["z"]) ));

  // $JD0: $jd of 0h UTC $time
  $sunRise = sunRise($JD0, $form["DeltaT"], $lon, $lat, $form["Zone"], 0);

  $form["SunTransit"] = HHMM($sunRise["transit"]);
  $form["sunRise"]    = HHMM($sunRise["rise"]);
  $form["SunSet"]     = HHMM($sunRise["set"]);

  $form["SunCivilTwilightMorning"]    = HHMM($sunRise["cicilTwilightMorning"]);
  $form["SunCivilTwilightEvening"]    = HHMM($sunRise["cicilTwilightEvening"]);
  $form["SunNauticalTwilightMorning"]    = HHMM($sunRise["nauticalTwilightMorning"]);
  $form["SunNauticalTwilightEvening"]    = HHMM($sunRise["nauticalTwilightEvening"]);
  $form["SunAstronomicalTwilightMorning"]    = HHMM($sunRise["astronomicalTwilightMorning"]);
  $form["SunAstronomicalTwilightEvening"]    = HHMM($sunRise["astronomicalTwilightEvening"]);

  $form["MoonLon"] = round1000($moonCoor["LON"]*RAD);
  $form["MoonLat"] = round1000($moonCoor["LAT"]*RAD);
  $form["MoonRA"]  = HHMM($moonCoor["ra"]*RAD/15);
  $form["MoonDec"] = round1000($moonCoor["dec"]*RAD);
  $form["MoonAz"]   = round100($moonCoor["az"]*RAD);
  $form["MoonAlt"]  = round10($moonCoor["alt"]*RAD+Refraction($moonCoor["alt"]));  // including refraction
  $form["MoonAge"] = round1000($moonCoor["moonAge"]*RAD);
  $form["MoonPhaseNumber"] = round1000($moonCoor["phase"]);
  $form["MoonPhase"]    = $moonCoor["moonPhase"];

  $form["MoonSign"]     = $moonCoor["sign"];
  $form["MoonDistance"] = round10($moonCoor["distance"]);
  $form["MoonDiameter"] = round100($moonCoor["diameter"]*RAD*60); // angular diameter in arc seconds

  // Calculate distance from the $observer (on the surface of earth) to the center of the moon
  $moonCart      = EquPolar2Cart($moonCoor["raGeocentric"], $moonCoor["decGeocentric"], $moonCoor["distance"]);
  $form["MoonDistanceObserver"] = round10( sqrt( sqr($moonCart["x"]-$observerCart["x"]) + sqr($moonCart["y"]-$observerCart["y"]) + sqr($moonCart["z"]-$observerCart["z"]) ));

  $moonRise = moonRise($JD0,$form["DeltaT"], $lon, $lat, $form["Zone"], 0);

  $form["MoonTransit"] = HHMM($moonRise["transit"]);
  $form["moonRise"]    = HHMM($moonRise["rise"]);
  $form["MoonSet"]     = HHMM($moonRise["set"]);
}


$form = array ();
Init ($form);
for ($i=0;$i<10000;$i++)
    Compute ($form);

foreach ($form as $key => $value) {


}

print ("Time = ". (microtime(true)-$t0));

?>
