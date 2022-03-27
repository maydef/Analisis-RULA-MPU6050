<html>
<head>
    <link rel="stylesheet" type="text/css" href="style_tabel.css">  
    
</head>

<div class="halaman">
	<h2>Between</h2>
	<p>Berikut adalah fasilitas untuk mencari data sesuai range waktu yang dibutuhkan</p>
</div>
<body>

<?php
include "connect.php";
?>
<table align="center">
	
		<tr>
			<form method="post" name="form1" target="_self">
                <td align="center">
                        format input (h:m:s)
                </td>

				<th>Cari : <input name="awal" type="text" size="9" maxlength="9"> s/d <input name="akhir" type="text" size="9" maxlength="9">
                    <input name="btnCari" type="submit" size="20" value="Filter">
                </th>
			</form>
		</tr>

</table>
<br>
<table cellspacing='0' align="center">
    <thead>
        <tr>
            <th>ID</th>
            <th>Range</th>
            <th>Skor</th>
            <th>Waktu Awal</th>
            <th>Waktu Akhir</th>
            <th>Delta Time</th>
        </tr>
    </thead>
    <tbody>
        <?php 
            if(isset($_POST['btnCari']))
            { 

                $awal= $_POST['awal'];
                $akhir= $_POST['akhir'];
                $query="SELECT * FROM tb_update WHERE (awal_update BETWEEN '$awal' AND '$akhir') AND (akhir_update BETWEEN '$awal' AND '$akhir')";
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
            }
            ?>
    </tbody>
</table>
<br><br>
</body>
</html>

