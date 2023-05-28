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
    if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) 
    {
      let username = getCookie('username');

      if(username == null || username == "nulluser"){
        alert("请输入正确的用户名和密码!");
      }
      else{
        // alert("登录成功!");
        window.location.replace("http://localhost:8888/index/index");
      }
    }

  };
});


function getCookie(name) {
  var cookies = document.cookie.split(';');
  for (var i = 0; i < cookies.length; i++) {
      var cookie = cookies[i];
      // 去除左侧空格
      while (cookie.charAt(0) == ' ') {
          cookie = cookie.substring(1);
      }
      // 匹配cookie名称
      if (cookie.indexOf(name) == 0) {
          return decodeURIComponent(cookie.substring(name.length + 1));
      }
  }
  // 没有找到对应的cookie，返回null
  return null;
}