<?php
    session_start();
    if(!isset($_SESSION['Name'])){
        header("Location: login.php");
    }
    header("Refresh: 120; url=index.php");
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="./css/bootstrap.css">
    <title>SmartLocker</title>
</head>
<body>
    <div class="container">
        <h1 class="py-5">Welcom to System: <?php echo $_SESSION['Name']?></h1>
        <table class="table table-striped">
            <thead>
                <tr>
                    <th scope="col">#</th>
                    <th scope="col">Message</th>
                    <th scope="col">Date and Time</th>
                </tr>
            </thead>
            <tbody>
            <?php 
                require("database_connect.php");
                $strSQL = "SELECT * FROM tblstudlockerinfo ORDER BY InfoID DESC";
                if($rsPersons = mysqli_query($con, $strSQL)){
                    if(mysqli_num_rows($rsPersons) > 0){
                        while($recPerson = mysqli_fetch_array($rsPersons)){
                            echo '
                                <tr>
                                    <th>'. $recPerson['InfoID']  . '</th>
                                    <td>'. $recPerson['MessageHistory']  . '</td>
                                    <td>'. $recPerson['DateHappen']  . '</td>
                                </tr>     
                            '; 
                        }
                        mysqli_free_result($rsPersons);
                    }
                    else{
                        echo '
                            <tr>
                                <td collspan="3" align="center">No record found</td>
                            </tr>'; 
                    }

                }
                else{
                    echo 'ERROR: Could not execute your request';

                }
                require("database_close.php"); 

            ?>


            </tbody>
        </table>
    </div>
    <script src="./js/jquery.js"></script>
    <script src="./js/bootstrap.js"></script>
</body>
</html>