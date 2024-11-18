let dropArea = document.getElementById("drop-area");

["dragenter", "dragover", "dragleave", "drop"].forEach((eventName) => {
    dropArea.addEventListener(eventName, preventDefaults, false);
    document.body.addEventListener(eventName, preventDefaults, false);
});

["dragenter", "dragover"].forEach((eventName) => {
    dropArea.addEventListener(eventName, highlight, false);
});

["dragleave", "drop"].forEach((eventName) => {
    dropArea.addEventListener(eventName, unhighlight, false);
});

dropArea.addEventListener("drop", handleDrop, false);

function preventDefaults(e) {
    e.preventDefault();
    e.stopPropagation();
}

function highlight(e) {
    dropArea.classList.add("highlight");
}

function unhighlight(e) {
    dropArea.classList.remove("highlight");
}

async function handleDrop(e) {
    let dt = e.dataTransfer;
    let files = dt.files;
    await handleFiles(files);
}

function uploadFile(file) {
    console.log("Uploading", file.name);
}

dropArea.addEventListener("click", () => {
    fileElem.click();
});

let fileElem = document.getElementById("fileElem");
fileElem.addEventListener("change", async function (e) {
    await handleFiles(this.files);
});
