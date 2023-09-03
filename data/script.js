// Retrieve JSON data from the JSON file in SPIFFS
fetch('igate_conf.json')
    .then(response => response.json())
    .then(data => {
        // Process the JSON data
        displayJSONData(data);
    })
    .catch(error => {
        console.error('Error fetching JSON:', error);
    });

// Display JSON data in the HTML
function displayJSONData(data) {
    const jsonContainer = document.getElementById('json-container');
    /*jsonContainer.innerHTML = '';

    data.forEach(item => {
        const itemDiv = document.createElement('div');
        itemDiv.textContent = JSON.stringify(item, null, 2);
        jsonContainer.appendChild(itemDiv);
    });*/

    jsonContainer.innerHTML = JSON.stringify(data, null, 2);
}
