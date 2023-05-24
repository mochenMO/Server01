

function info(id, val) {
    this.id = id;
    this.val = val;
}






let hashMap = {
    "btn1_1": { flag: 1, canInput:true },
    "btn1_2": { flag: 1, canInput: true },
    "btn1_3": { flag: 1, canInput: true },
    "btn2_1": { flag: 1, canInput: false },
    "btn2_2": { flag: 1, canInput: false },
    "btn2_3": { flag: 1, canInput: false },
    "btn2_4": { flag: 1, canInput: false },
    "btn3_1": { flag: 1, canInput: false },
    "btn3_2": { flag: 1, canInput: false },
    "btn4_1": { flag: 1, canInput: false },
    "btn4_2": { flag: 1, canInput: false},
    "btn4_3": { flag: 1, canInput: false},
};






function AddButtonEvent() {



    let keyArr = Object.keys(hashMap);

    for (let i = 0; i < keyArr.length; i++) {

        let id = keyArr[i];
        let mySpan = document.getElementById(id);

        mySpan.addEventListener('click', () => {

            let newImg = mySpan.querySelector("img");
            if (hashMap[id].flag == 1) {
                // 获取模板
                const template = document.querySelector('#testbox');

                // 克隆模板内容并填充数据
                const instance = document.importNode(template.content, true);
                let container = document.querySelector('#box3');

                let tempID = "box3" + "_" + id;
                instance.querySelector("div").id = tempID;
                instance.querySelector("p").textContent = mySpan.textContent;


                // 设置input
                if (hashMap[id].canInput == false) {
                    instance.querySelector("input[type=\"text\"]").disabled = true;
                }

                container.appendChild(instance);
                newImg.src = "../index/res/box2.png";
            }
            else {

                var element = document.getElementById("box3" + "_" + id);
                element.parentNode.removeChild(element);


                newImg.src = "../index/res/box1.png";
            }
            hashMap[id].flag = -hashMap[id].flag;


            // console.log("asdad");
        })
    }

}


document.getElementById("run").addEventListener("click", () => {
   
    let requestData = {
        "length": "0",
        "Data": []
    }

    let len = 0;

    let minBoxArr = document.querySelectorAll("#box3 .minbox");
    // console.log(minBoxArr);
    if (minBoxArr.length == 0) {
        alert("还未添加任何算法，程序无法运行!");
        return;
    }

    for (let i = 0; i < minBoxArr.length; i++) {

        let id = minBoxArr[i].id;
        let myInputText = minBoxArr[i].querySelector("input[type=\"text\"]");
        let val = myInputText.value;

        let pattern = /^-?\d+(\.\d+)?$/;
        if (pattern.test(val) == false && hashMap[id.slice(5)].canInput == true) {
            alert(minBoxArr[i].innerText + "算法,参数输入错误!");
            return;
        } else {

            let tempData = {
                funcName: id,
                parameter: val
            };

            requestData["Data"].push(tempData);
            ++len;
        }
        // console.log(myInputText);
        // console.log(myInputText.value);
        // console.log(minBoxArr[i]); 
    }
    requestData["length"] = len;

    console.log(requestData);   /// 


    // 把数据发给服务器
    let xhr = new XMLHttpRequest(); // 创建一个新的 XHR 对象
    xhr.open('POST', 'http://localhost:8888/index/index.json'); // 配置 XHR 对象
    xhr.setRequestHeader('Content-Type', 'application/json'); // 声明请求头部类型 
    xhr.send(JSON.stringify(requestData)); // 将 JavaScript 对象转换成 JSON 字符串并发送请求

});







function main() {
    AddButtonEvent();


}

main();



