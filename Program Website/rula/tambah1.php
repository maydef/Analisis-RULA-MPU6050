<script type="text/javascript">
date = new Date();
millisecond = date.getMilliseconds();
detik = date.getSeconds();
menit = date.getMinutes();
jam = date.getHours();
hari = date.getDay();
tanggal = date.getDate();
bulan = date.getMonth();
tahun = date.getFullYear();
document.write(tanggal+"-"+arrbulan[bulan]+"-"+tahun+"<br/>"+jam+" : "+menit+" : "+detik+"."+millisecond);</script>
<?php
$jam="<script>document.writeln(jam);</script>";
$menit="<script>document.writeln(menit);</script>";
$detik="<script>document.writeln(detik);</script>";
$mili="<script>document.writeln(millisecond);</script>";
$waktu=$jam.":".$menit.":".$detik.":".$mili;
$satu = $_GET['pertama'];
$dua = $_GET['kedua'];
mysql_query("INSERT INTO coba SET data1='$satu',data2='$dua', mili='$waktu'");
?>