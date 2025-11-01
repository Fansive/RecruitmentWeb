function saveProfile() {
    const form = document.getElementById("profileForm");
    const formData = new FormData(form);

    const userInfo = {};
    formData.forEach((value, key) => {
        userInfo[key] = value;
    });

    console.log("保存的个人信息:", userInfo);

    alert("个人信息已保存");
}

function goBack() {
    history.back();
}

// function goTo(page) {
//     if (page === "profile") location.href = "profile.html";
//     else if (page === "records") alert("��ת��Ͷ�ݼ�¼ҳ�棨��ʵ�֣�");
// }
