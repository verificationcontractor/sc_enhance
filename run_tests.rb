require 'fileutils'

tests = `ls tests | grep test_ | grep -v cpp | awk -F'.' '{print $1}'`.split "\n"

puts "Building tests..."
`make build_tests`

tests.each do |it|
  puts "Running #{it}"
  `tests/#{it} > tests/#{it}.log`
  if FileUtils.identical?("tests/#{it}.log", "tests/#{it}.golden")
    puts("RESULT: #{it} PASSED")
  else
    puts("RESULT: #{it} FAILED")
  end
end

puts "Cleaning up mess..."
`make clean`
puts "Done."
