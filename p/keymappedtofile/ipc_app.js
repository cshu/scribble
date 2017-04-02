
//undone try to avoid chunked encoding because it's inconvenient. you can even always use GET to transfer data within custom headers! (note X- for custom header naming is deprecated. Yet you can use it.)

//undone set setRequestHeader('Content-Type','text/plain; charset=UTF-8')?
//undone setting responseType of xhr needed? even if you can set it, the only value you can set is "text"? IE only supports it from IE 10 or above?

//undone if you use jquery, the contentType property is 'application/x-www-form-urlencoded; charset=UTF-8' by default, you can change to text/plain, but does that matter?
//undone if you use jquery, the dataType property is for setting date type to expect from response (jquery: "The type of pre-processing depends by default upon the Content-Type of the response, but can be set explicitly using the dataType option. If the dataType option is provided, the Content-Type header of the response will be disregarded.")

