<?php
    session_start();
    if(isset($_SESSION['Name'])){
        header("Location: index.php");
    }

    if(isset($_POST['btnLogin'])){
        require("database_connect.php");
        $studentno = htmlspecialchars( $_POST['studentno']); // anti xss
        $password = htmlspecialchars( $_POST['password']);

        $studentno = stripslashes($studentno);    // removal of single quotes, slash specifically
        $password = stripslashes($password);

        $studentno = mysqli_real_escape_string($con, $studentno); //escaping any attempts for SQL Injection
        $password = mysqli_real_escape_string($con, $password);

        $password = md5($password); // hash the password

        $strSQL = "
                    SELECT * FROM tblstudacc 
                    WHERE StudentNo = '$studentno' 
                    AND Password = '$password'
        
        ";
        if($rsLogin = mysqli_query($con, $strSQL)){
            if(mysqli_num_rows($rsLogin) > 0){
                $rec = mysqli_fetch_array($rsLogin);
                $_SESSION['Name'] = $rec['FullName'];
                header("Location: index.php");
                mysqli_free_result($rsLogin);
            }
            else{
                echo '
                
                <div class="alert alert-danger alert-dismissible fade show" role="alert">
                    <strong>Wrong</strong> Username/Password.
                    <button type="button" class="close" data-dismiss="alert" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                </div>
                
                '; 
            }
        }
        else
            echo 'ERROR: Could not execute your request';
        
        require("database_close.php"); 
    }

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="./css/bootstrap.css">
    <link rel="stylesheet" href="./css/custom-login.css">
    <title>Smart | Login</title>
</head>
<body>
<div class="alert alert-warning alert-dismissible fade show" role="alert">
  <strong>Holy guacamole!</strong> You should check in on some of those fields below.
  <button type="button" class="close" data-dismiss="alert" aria-label="Close">
    <span aria-hidden="true">&times;</span>
  </button>
</div>
    <div class="wrapper fadeInDown">
        <div id="formContent" class="py-5">
            <!-- Tabs Titles -->
            <div class="fadeIn first">
            <h1>STUDENT LOCKER</h1>
        </div>
        
        <!-- Login Form -->
        <form method="post">
            <input type="text" id="login" class="fadeIn second" name="studentno" placeholder="Student No">
            <input type="text" id="login" class="fadeIn third" name="password" placeholder="Password">
            <input type="submit" name="btnLogin"class="fadeIn fourth" value="Log In">
        </form>
    </div>


    <script src="./js/jquery.js"></script>
    <script src="./js/bootstrap.js"></script>
</body>
</html>