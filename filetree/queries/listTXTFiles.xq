declare variable $where as xs:string := string($fileTree/@filePath);
<html>
  <head>
    <title>All txt files in: {$where}</title>
  </head>
  <body>
    <p>
      txt-files found in {$where} sorted by size:
    </p>
    <ul> {
      for $file in $fileTree//file[@suffix = "txt"]
      order by xs:integer($file/@size)
      return
        <li>
          {string($file/@fileName)}, size: {string($file/@size)}
        </li>
    } </ul>
  </body>
</html>
