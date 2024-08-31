document.addEventListener("DOMContentLoaded", function() {
    let image_off = document.getElementById("led-off");
    let image_on  = document.getElementById("led-on");
    let button    = document.getElementById("led_button");

    button.onclick = function() {
        if (image_off.style.visibility === 'visible') {
            led('ON')
                .then(response => {
                    if (response === 'done') {
                        image_off.style.visibility = 'hidden';
                        image_on.style.visibility = 'visible';
                    } else {
                        console.error('Unexpected response:', response);
                    }
                });
        } else {
            led('OFF')
                .then(response => {
                    if (response === 'done') {
                        image_off.style.visibility = 'visible';
                        image_on.style.visibility = 'hidden';
                    } else {
                        console.error('Unexpected response:', response);
                    }
                });
        }
    };
});



function led(state) {
    let url = "http://192.168.1.4:8080/" + state;
    console.log(url);
    
    return fetch(url, {
        method: 'GET'
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        const contentType = response.headers.get('Content-Type');

        if (contentType && contentType.includes('text/html')) {
    
            return response.text().then(html => {
                document.open(); 
                document.write(html);
                document.close();
            });
        }
        return response.text();  
    })
    .then(text => {
        return text;  
    })
    .catch(error => {
        console.error('Error:', error);
        return 'error';  
    });
}
