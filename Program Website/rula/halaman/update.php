<html>
<head>
    <link rel="stylesheet" type="text/css" href="style_tabel.css">  
    <meta http-equiv="refresh" content="5"/>
</head>

<div class="halaman">
	<h2>Update Perhitungan</h2>
	<p>Berikut adalah nilai update perhitungan alat ukur sudut punggung menggunakan MPU6050</p>
</div>
<body>

<?php
include "connect.php";
?>

<table cellspacing='0' align="center">
        <thead>
            <tr>
                <th>ID Update</th>
                <th>Range</th>
                <th>Skor</th>
                <th>Waktu Awal</th>
                <th>Waktu Akhir</th>
                <th>Delta Time</th>
            </tr>
        </thead>
        <tbody>  
        <?php
        $query = "SELECT * FROM tb_update";
        $sql = mysql_query($query); 
        while($hasil=mysql_fetch_array($sql))
        {
        ?>    
            <tr>
                <td><?php echo $hasil ['id_update']; ?></td>
                <td><?php echo $hasil ['range_update']; ?></td>
                <td><?php echo $hasil ['skor_update']; ?></td>
                <td><?php echo $hasil ['awal_update']; ?></td>
                <td><?php echo $hasil ['akhir_update']; ?></td>
                <td><?php echo $hasil ['dt_update']; ?></td>
            </tr>
        <?php
        }
        ?>
        </tbody>
    </table>
    <br>
    <br>
</body>

</html>

