<?php
include "connect.php";
$range = $_GET['range'];
$skor = $_GET['skor'];
$awal = $_GET['awal'];
$akhir = $_GET['akhir'];
$dt = $_GET['dt'];

mysql_query("INSERT INTO tb_update SET range_update='$range',skor_update='$skor',awal_update='$awal',akhir_update='$akhir',dt_update='$dt'");
?>