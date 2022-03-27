<html>
<head>
    <link rel="stylesheet" type="text/css" href="style_tabel.css">  
    <meta http-equiv="refresh" content="5"/>
</head>

<div class="halaman">
	<h2>Aktivitas Alat Ukur</h2>
	<p>Berikut adalah tabel aktivitas alat pendeteksi berdasarkan aktivasi alat ukur</p>
</div>

<?php
include "connect.php";
?>

<body>
<table cellspacing='0' align="center">
        <thead>
            <tr>
                <th>ID Aktivitas</th>
                <th>Waktu Awal Aktivitas</th>
                <th>Waktu Akhir Aktivitas</th>
            </tr>
        </thead>
        <tbody>  
        <?php
        $query = "SELECT * FROM tb_aktivitas";
        $sql = mysql_query($query); 
        while($hasil=mysql_fetch_array($sql))
        {
        ?>    
            <tr>
                <td><?php echo $hasil ['id_aktivitas']; ?></td>
                <td><?php echo $hasil ['awal_aktivitas']; ?></td>
                <td><?php echo $hasil ['akhir_aktivitas']; ?></td>
            </tr>
        <?php
        }
        ?>
        </tbody>
    </table>
    <br>
        <br>
    </body>