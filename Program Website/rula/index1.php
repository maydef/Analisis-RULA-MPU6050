<!DOCTYPE html>
<html>
<head>
	<title>.: MONITORING RULA :.</title>
	<!-- menghubungkan dengan file css -->
	<link rel="stylesheet" type="text/css" href="style_desain.css">
	<!-- menghubungkan dengan file jquery -->
	<script type="text/javascript" src="jquery.js"></script>
</head>
<body>

<div class="content">
	<header>
		<h1 class="judul">MONITORING RULA</h1>
		<h3 class="deskripsi">Alat Ukur Kebungkukan Punggung Dengan MPU6050 GY-521</h3>
	</header>

	<div class="menu">
		<ul>
			<li><a href="index.php?page=abstrak">ABSTRAK</a></li>
			<li><a href="index.php?page=aktivitas">AKTIVITAS</a></li>
			<li><a href="index.php?page=update">UPDATE</a></li>
			<li><a href="index.php?page=between">BETWEEN</a></li>
		</ul>
	</div>

	<div class="badan">


	<?php 
	if(isset($_GET['page'])){
		$page = $_GET['page'];

		switch ($page) {
			case 'abstrak':
				include "halaman/abstrak.php";
				break;
			case 'aktivitas':
				include "halaman/aktivitas.php";
				break;
			case 'update':
				include "halaman/update.php";
				break;		
			case 'between':
				include "halaman/between.php";
				break;	
			default:
				echo "<center><h3>Maaf. Halaman tidak di temukan !</h3></center>";
				break;
		}
	}else{
		include "halaman/abstrak.php";
	}

	 ?>

	</div>
</div>
</body>
</html>