<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <body>
  <h2>Working Hours. PSC</h2>
  <table border="1">
    <tr>
      <th>Start</th>
      <th>End</th>
      <th>Client</th>
      <th>Duration</th>
      <th>Task</th>
    </tr>
    <xsl:for-each select="year/day/task">
    <xsl:sort select="@client"/>
    <tr>
      <td><xsl:value-of select="@start"/></td>
      <td><xsl:value-of select="@end"/></td>
      <td><xsl:value-of select="@client"/></td>
      <td><xsl:value-of select="@client"/></td>
      <td><xsl:value-of select="@name"/></td>
    </tr>
    </xsl:for-each>
  </table>
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>
