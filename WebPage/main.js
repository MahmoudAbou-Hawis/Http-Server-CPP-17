document.addEventListener("DOMContentLoaded", function() {
  let LoginButtonElement  = document.getElementById("login");
  let UserNameElement = document.getElementById("Username");
  let UserPasswordElement = document.getElementById("password");


  if (LoginButtonElement) {
          LoginButtonElement.onclick = function() {
          let usernameValue = UserNameElement.value; 
          let UserPasswordValue = UserPasswordElement.value; 
          if(checkTheFiledsNotEmpty(usernameValue,UserPasswordValue))
          {
            document.getElementById("user-login-error").innerText = "Please fill the fields it's empety";
          }
          else if(sendDataToTheServer(usernameValue,UserPasswordValue))
          {
            
          }
          else
          {
            document.getElementById("user-login-error").innerText = "wrong password or username";
          }
      };
  }
});

function checkTheFiledsNotEmpty(name, pass)
{
  return (name.length == 0 || pass.length == 0);
}

function sendDataToTheServer(name, pass)
{
  let url = 'http://192.168.1.4:8080/login?';
  url+= 'name='+ name;
  url+= '&pass='+ pass;
  
  fetch(url, {
    method: 'GET'
})
.then(response => {

    const contentType = response.headers.get('Content-Type');

    if (contentType && contentType.includes('text/html')) {

        return response.text().then(html => {
            document.open(); 
            document.write(html);
            document.close();
        });
    } else if (contentType && contentType.includes('text/plain')) {
        return response.text().then(text => {
          document.getElementById("user-login-error").innerText = text;
        });
    } else {
        throw new Error('Unexpected content type: ' + contentType);
    }
})
.catch(error => {
    console.error('Error:', error);
});
return true;
}