var submitBtn = document.getElementById("myButton");
var usernameInput = document.getElementById('userName');
var passwordInput = document.getElementById('passWord');

submitBtn.addEventListener('click', function(e) {
  
  console.log(usernameInput.value);  /////
  console.log(passwordInput.value);  /////

  e.preventDefault(); // 防止表单自动刷新

  var xhr = new XMLHttpRequest(); // 创建一个新的 XHR 对象
  
  xhr.open('POST', 'http://localhost:8888/login/login.json'); // 配置 XHR 对象
  xhr.setRequestHeader('Content-Type', 'application/json'); // 声明请求头部类型 

  var userData = {
    username: usernameInput.value,
    password: passwordInput.value,
    "click":"login"
  };

  xhr.send(JSON.stringify(userData)); // 将 JavaScript 对象转换成 JSON 字符串并发送请求

  xhr.onreadystatechange = function() { // 绑定回调函数
    if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
      console.log("登录成功！");
      //location.href="../index/index.html";
    }
    else {
      console.log("出错了，请重试！")
    }
  };
});


